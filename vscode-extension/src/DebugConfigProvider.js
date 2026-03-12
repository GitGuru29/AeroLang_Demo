'use strict';
// ─────────────────────────────────────────────────────────────────────────────
//  DebugConfigProvider.js
//  Auto-generates launch.json for AeroLang debug sessions.
// ─────────────────────────────────────────────────────────────────────────────

const vscode = require('vscode');
const path = require('path');
const fs = require('fs');

class AeroDebugConfigProvider {

    /**
     * Called when the user clicks "Run and Debug" with no launch.json.
     * We build a default config from the active .aero file.
     */
    provideDebugConfigurations(folder, token) {
        return [this._makeConfig(folder ? folder.uri.fsPath : '')];
    }

    /**
     * Called when VS Code resolves a launch config before starting a session.
     * We fill in any missing fields.
     */
    resolveDebugConfiguration(folder, config, token) {
        // New session with empty config — build default
        if (!config.type && !config.request && !config.name) {
            const activeFile = vscode.window.activeTextEditor?.document?.fileName;
            if (activeFile && activeFile.endsWith('.aero')) {
                return this._makeConfig(folder ? folder.uri.fsPath : path.dirname(activeFile), activeFile);
            }
            vscode.window.showErrorMessage('Open a .aero file first to start debugging.');
            return undefined;
        }

        // Fill in defaults for an existing config
        if (!config.program) {
            const activeFile = vscode.window.activeTextEditor?.document?.fileName;
            if (activeFile && activeFile.endsWith('.aero')) {
                config.program = activeFile;
            } else {
                vscode.window.showErrorMessage('Set "program" to the .aero file to debug.');
                return undefined;
            }
        }

        config.workspaceRoot = config.workspaceRoot
            || (folder ? folder.uri.fsPath : path.dirname(config.program));

        // Locate gdb from NDK if not user-specified
        if (!config.gdbPath) {
            config.gdbPath = this._findNdkGdb(config.workspaceRoot) || 'gdb';
        }

        return config;
    }

    // ── Helpers ────────────────────────────────────────────────────────────────

    _makeConfig(workspaceRoot, program) {
        // Default program = first .aero file in src/
        if (!program) {
            const srcDir = path.join(workspaceRoot, 'src');
            if (fs.existsSync(srcDir)) {
                const aeroFiles = fs.readdirSync(srcDir).filter(f => f.endsWith('.aero'));
                if (aeroFiles.length > 0) {
                    program = path.join(srcDir, aeroFiles[0]);
                }
            }
        }

        return {
            type: 'aerolang',
            request: 'launch',
            name: 'Debug AeroLang App',
            program: program || '${file}',
            workspaceRoot: workspaceRoot || '${workspaceFolder}',
            device: '',       // ADB serial (empty = first connected device)
            gdbPath: '',       // path to gdb/lldb (auto-detected from NDK if empty)
            stopOnEntry: false,
        };
    }

    /** Try to locate the gdb binary shipped with the Android NDK. */
    _findNdkGdb(workspaceRoot) {
        // Candidates: env var, local.properties, common install paths
        const ndkHome = process.env.ANDROID_NDK_HOME
            || process.env.ANDROID_NDK_ROOT
            || this._readLocalPropertiesNdk(workspaceRoot);

        if (!ndkHome) return null;

        // NDK ships gdb at <ndk>/prebuilt/<host>/bin/gdb
        const hostTag = process.platform === 'win32' ? 'windows-x86_64'
            : process.platform === 'darwin' ? 'darwin-x86_64'
                : 'linux-x86_64';
        const gdbPath = path.join(ndkHome, 'prebuilt', hostTag, 'bin', 'gdb');
        return fs.existsSync(gdbPath) ? gdbPath : null;
    }

    /** Read ndk.dir from local.properties (standard Android project convention) */
    _readLocalPropertiesNdk(workspaceRoot) {
        try {
            const lp = fs.readFileSync(path.join(workspaceRoot, 'local.properties'), 'utf8');
            const m = lp.match(/^ndk\.dir=(.+)$/m);
            return m ? m[1].trim() : null;
        } catch {
            return null;
        }
    }
}

module.exports = AeroDebugConfigProvider;
