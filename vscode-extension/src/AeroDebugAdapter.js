'use strict';
// ─────────────────────────────────────────────────────────────────────────────
//  AeroDebugAdapter.js
//  VS Code Debug Adapter for AeroLang (Debug Adapter Protocol).
//
//  Architecture:
//   VS Code ↔── DAP (stdio) ──▶ AeroDebugAdapter ──▶ GDB/LLDB (via ADB)
//                                        ↕
//                               SourceMapReader (.aero.srcmap)
//
//  Translate .aero line ↔ generated C++ line for all breakpoint/frame ops.
// ─────────────────────────────────────────────────────────────────────────────

const { DebugSession, InitializedEvent, TerminatedEvent,
    StoppedEvent, OutputEvent, BreakpointEvent,
    StackFrame, Scope, Source, Handles,
    Breakpoint } = require('@vscode/debugadapter');
const { spawn } = require('child_process');
const { EventEmitter } = require('events');
const path = require('path');
const fs = require('fs');

const SourceMapReader = require('./SourceMapReader');

// ─── GDB/MI wrapper ──────────────────────────────────────────────────────────

class GdbSession extends EventEmitter {
    constructor() {
        super();
        this._proc = null;
        this._pending = new Map(); // token → { resolve, reject }
        this._token = 1;
        this._buffer = '';
    }

    /** Start gdb --interpreter=mi2, resolve when it's ready */
    start(gdbPath, program) {
        return new Promise((resolve, reject) => {
            this._proc = spawn(gdbPath, ['--interpreter=mi2', '--quiet', program]);
            this._proc.stdout.on('data', (data) => this._onData(data.toString()));
            this._proc.stderr.on('data', (data) => this.emit('output', data.toString(), 'stderr'));
            this._proc.on('exit', () => this.emit('exit'));

            // Wait for initial prompt
            const onReady = (text) => {
                if (text.includes('(gdb)')) {
                    this.off('output', onReady);
                    resolve();
                }
            };
            this.on('output', onReady);
            setTimeout(() => reject(new Error('GDB start timeout')), 10000);
        });
    }

    /** Send a GDB/MI command, return parsed result record */
    send(cmd) {
        const token = this._token++;
        const line = `${token}-${cmd}\n`;
        return new Promise((resolve, reject) => {
            this._pending.set(token, { resolve, reject });
            this._proc.stdin.write(line);
        });
    }

    kill() {
        if (this._proc) this._proc.kill();
    }

    _onData(chunk) {
        this._buffer += chunk;
        const lines = this._buffer.split('\n');
        this._buffer = lines.pop(); // keep incomplete last line
        for (const line of lines) {
            this._parse(line.trim());
        }
    }

    _parse(line) {
        if (!line) return;
        this.emit('output', line + '\n', 'console');

        // Result record: token^done|error|...
        const resultMatch = line.match(/^(\d+)\^(done|error|running|connected|exit)(,(.*))?$/);
        if (resultMatch) {
            const token = parseInt(resultMatch[1]);
            const cls = resultMatch[2];
            const data = resultMatch[4] || '';
            const entry = this._pending.get(token);
            if (entry) {
                this._pending.delete(token);
                if (cls === 'error') {
                    entry.reject(new Error(data));
                } else {
                    entry.resolve({ class: cls, data });
                }
            }
            return;
        }

        // Async stop record: *stopped,...
        if (line.startsWith('*stopped')) {
            this.emit('stopped', line);
        }
    }
}

// ─── parseKV helpers ──────────────────────────────────────────────────────────

function parseIntField(data, field) {
    const m = data.match(new RegExp(`${field}="(\\d+)"`));
    return m ? parseInt(m[1]) : 0;
}

function parseStrField(data, field) {
    const m = data.match(new RegExp(`${field}="([^"]*)"`));
    return m ? m[1] : '';
}

// ─── Debug Adapter ────────────────────────────────────────────────────────────

class AeroDebugAdapter extends DebugSession {
    constructor() {
        super();
        this._gdb = new GdbSession();
        this._srcMap = new SourceMapReader();
        this._handles = new Handles();
        this._breakpoints = new Map(); // aeroFile → [{ aeroLine, bpId }]
        this._aeroFile = '';     // currently debugged .aero file
        this._cppFile = '';     // the generated .cpp
        this._workspaceRoot = '';

        // 1-based lines/columns
        this.setDebuggerLinesStartAt1(true);
        this.setDebuggerColumnsStartAt1(true);
    }

    // ── Lifecycle ──────────────────────────────────────────────────────────────

    initializeRequest(response, args) {
        response.body = response.body || {};
        response.body.supportsConfigurationDoneRequest = true;
        response.body.supportsEvaluateForHovers = true;
        response.body.supportsSetVariable = false;
        response.body.supportsRestartRequest = false;
        this.sendResponse(response);
        this.sendEvent(new InitializedEvent());
    }

    async launchRequest(response, args) {
        this._aeroFile = args.program;      // absolute path to .aero file
        this._workspaceRoot = args.workspaceRoot || path.dirname(this._aeroFile);
        const device = args.device || '';  // ADB device serial
        const gdbPath = args.gdbPath || 'gdb';

        // Determine generated .cpp from source map
        this._srcMap.invalidate(this._aeroFile);
        this._cppFile = this._srcMap.getCppFile(this._aeroFile)
            || this._aeroFile.replace('.aero', '.cpp');

        try {
            this.sendEvent(new OutputEvent(`[AeroLang] Starting debugger for ${path.basename(this._aeroFile)}\n`));
            await this._gdb.start(gdbPath, this._cppFile);

            this._gdb.on('stopped', (record) => this._onStopped(record));
            this._gdb.on('output', (text, cat) => {
                if (cat === 'console') return; // we swallow raw GDB console noise
                this.sendEvent(new OutputEvent(text, cat));
            });
            this._gdb.on('exit', () => this.sendEvent(new TerminatedEvent()));

            this.sendResponse(response);
        } catch (e) {
            response.success = false;
            response.message = e.message;
            this.sendResponse(response);
        }
    }

    configurationDoneRequest(response) {
        this.sendResponse(response);
        // Start execution
        this._gdb.send('exec-run').catch(() => { });
    }

    async disconnectRequest(response) {
        this._gdb.kill();
        this.sendResponse(response);
    }

    // ── Breakpoints ───────────────────────────────────────────────────────────

    async setBreakpointsRequest(response, args) {
        const aeroFile = args.source.path;

        // Clear existing breakpoints for this file
        const existing = this._breakpoints.get(aeroFile) || [];
        for (const bp of existing) {
            try { await this._gdb.send(`break-delete ${bp.bpId}`); } catch { }
        }
        this._breakpoints.set(aeroFile, []);

        const confirmed = [];
        for (const srcBp of (args.breakpoints || [])) {
            const aeroLine = srcBp.line;
            const cppLine = this._srcMap.aeroToGen(aeroFile, aeroLine);

            if (cppLine < 1) {
                // No mapping — mark unverified
                confirmed.push(new Breakpoint(false));
                continue;
            }

            try {
                const res = await this._gdb.send(
                    `break-insert ${this._cppFile}:${cppLine}`
                );
                const bpId = parseIntField(res.data, 'number');
                this._breakpoints.get(aeroFile).push({ aeroLine, bpId });
                const bp = new Breakpoint(true, aeroLine, 1, new Source(path.basename(aeroFile), aeroFile));
                confirmed.push(bp);
            } catch (e) {
                confirmed.push(new Breakpoint(false));
            }
        }

        response.body = { breakpoints: confirmed };
        this.sendResponse(response);
    }

    // ── Execution control ─────────────────────────────────────────────────────

    continueRequest(response) { this._gdb.send('exec-continue'); this.sendResponse(response); }
    nextRequest(response) { this._gdb.send('exec-next'); this.sendResponse(response); }
    stepInRequest(response) { this._gdb.send('exec-step'); this.sendResponse(response); }
    stepOutRequest(response) { this._gdb.send('exec-finish'); this.sendResponse(response); }
    pauseRequest(response) { this._gdb.send('exec-interrupt'); this.sendResponse(response); }

    // ── Stack / Variables ─────────────────────────────────────────────────────

    async stackTraceRequest(response, args) {
        try {
            const res = await this._gdb.send('stack-list-frames');
            const frames = [];

            // Parse frame records: {level="0",addr="...",func="...",file="...",fullname="...",line="..."}
            const frameRe = /\{[^}]+\}/g;
            let m;
            while ((m = frameRe.exec(res.data)) !== null) {
                const fd = m[0];
                const level = parseIntField(fd, 'level');
                const func = parseStrField(fd, 'func');
                const file = parseStrField(fd, 'fullname') || this._cppFile;
                const cppLn = parseIntField(fd, 'line');

                // Translate back to .aero
                const aeroLn = this._srcMap.genToAero(this._aeroFile, cppLn);
                const srcFile = aeroLn > 0 ? this._aeroFile : file;
                const dispLine = aeroLn > 0 ? aeroLn : cppLn;

                frames.push(new StackFrame(
                    this._handles.create({ level }),
                    func || '<anonymous>',
                    new Source(path.basename(srcFile), srcFile),
                    dispLine
                ));
            }

            response.body = { stackFrames: frames, totalFrames: frames.length };
        } catch {
            response.body = { stackFrames: [], totalFrames: 0 };
        }
        this.sendResponse(response);
    }

    scopesRequest(response, args) {
        const frameId = this._handles.get(args.frameId);
        const level = frameId ? frameId.level : 0;
        response.body = {
            scopes: [
                new Scope('Locals', this._handles.create({ type: 'locals', level }), false),
            ]
        };
        this.sendResponse(response);
    }

    async variablesRequest(response, args) {
        const ref = this._handles.get(args.variablesReference);
        const level = ref ? ref.level : 0;
        const vars = [];

        try {
            // Select the correct frame
            await this._gdb.send(`stack-select-frame ${level}`);
            const res = await this._gdb.send('stack-list-locals --simple-values');

            // Parse: name="x",value="42"
            const varRe = /\{[^}]+\}/g;
            let m;
            while ((m = varRe.exec(res.data)) !== null) {
                const vd = m[0];
                vars.push({
                    name: parseStrField(vd, 'name'),
                    value: parseStrField(vd, 'value') || '<complex>',
                    variablesReference: 0,
                });
            }
        } catch { }

        response.body = { variables: vars };
        this.sendResponse(response);
    }

    async evaluateRequest(response, args) {
        try {
            const res = await this._gdb.send(`data-evaluate-expression "${args.expression}"`);
            response.body = {
                result: parseStrField(res.data, 'value') || res.data,
                variablesReference: 0,
            };
        } catch (e) {
            response.body = { result: e.message, variablesReference: 0 };
        }
        this.sendResponse(response);
    }

    threadsRequest(response) {
        response.body = { threads: [{ id: 1, name: 'main' }] };
        this.sendResponse(response);
    }

    // ── Internal ──────────────────────────────────────────────────────────────

    _onStopped(record) {
        // *stopped,reason="breakpoint-hit",...,line="42"
        const reason = parseStrField(record, 'reason') || 'pause';
        const cppLine = parseIntField(record, 'line');
        const aeroLn = cppLine > 0 ? this._srcMap.genToAero(this._aeroFile, cppLine) : 0;

        const ev = new StoppedEvent(reason.replace(/-/g, ' '), 1);
        this.sendEvent(ev);

        if (aeroLn > 0) {
            this.sendEvent(new OutputEvent(
                `[AeroLang] Stopped at ${path.basename(this._aeroFile)}:${aeroLn}\n`
            ));
        }
    }
}

// ── Entry point ───────────────────────────────────────────────────────────────
AeroDebugAdapter.run(AeroDebugAdapter);
