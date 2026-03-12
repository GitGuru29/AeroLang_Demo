const vscode = require('vscode');
const { spawn } = require('child_process');
const path = require('path');
const fs = require('fs');

class BuildManager {
    constructor(context) {
        this.context = context;
        this.outputChannel = vscode.window.createOutputChannel('AeroLang Build');
    }

    /**
     * Resolve the path to the aero-build script, searching common locations.
     */
    _findAeroBuild(projectPath) {
        const candidates = [
            // Sibling tools directory (workspace is inside a project)
            path.join(projectPath, '..', 'tools', 'aero-build'),
            // Tools directory inside the workspace
            path.join(projectPath, 'tools', 'aero-build'),
            // Global install
            'aero-build',
        ];
        for (const c of candidates) {
            try {
                if (c === 'aero-build') return c; // assume it's in PATH
                if (fs.existsSync(c)) return c;
            } catch (_) { }
        }
        return null;
    }

    /**
     * Build all .aero sources in the workspace via the aero-build script.
     */
    async buildProject() {
        const workspaceFolder = vscode.workspace.workspaceFolders?.[0];
        if (!workspaceFolder) {
            vscode.window.showErrorMessage('AeroLang: No workspace folder open.');
            return false;
        }

        const projectPath = workspaceFolder.uri.fsPath;
        const aeroBuild = this._findAeroBuild(projectPath);

        const config = vscode.workspace.getConfiguration('aerolang');
        const ndkPath = config.get('androidNdkPath') || '';
        const sdkPath = config.get('androidSdkPath') || '';

        // Build the command arguments
        const args = [projectPath];
        if (ndkPath.trim()) args.push('--ndk', ndkPath.trim());
        if (sdkPath.trim()) args.push('--sdk', sdkPath.trim());

        this.outputChannel.show(true);
        this.outputChannel.clear();
        this.outputChannel.appendLine('━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━');
        this.outputChannel.appendLine('  AeroLang Build System');
        this.outputChannel.appendLine('━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n');

        return vscode.window.withProgress({
            location: vscode.ProgressLocation.Notification,
            title: 'AeroLang: Building project...',
            cancellable: true,
        }, (progress, token) => new Promise((resolve) => {

            progress.report({ message: 'Starting build pipeline...' });

            const proc = spawn('bash', [aeroBuild, ...args], {
                cwd: projectPath,
                env: { ...process.env },
            });

            // Stream stdout live (strip ANSI colour codes for readability in the channel)
            proc.stdout.on('data', (data) => {
                const text = data.toString().replace(/\x1B\[[0-9;]*m/g, '');
                this.outputChannel.append(text);

                // Update progress message based on step markers
                const stepMatch = text.match(/\[(\d+)\/5\]\s+(.+)/);
                if (stepMatch) {
                    progress.report({ message: stepMatch[2].trim() });
                }
            });

            proc.stderr.on('data', (data) => {
                const text = data.toString().replace(/\x1B\[[0-9;]*m/g, '');
                this.outputChannel.append(`ERROR: ${text}`);
            });

            proc.on('close', (code) => {
                if (code === 0) {
                    this.outputChannel.appendLine('\n✅ Build succeeded!\n');
                    vscode.window.showInformationMessage(
                        '✅ AeroLang build complete!',
                        'Show Output'
                    ).then(sel => { if (sel) this.outputChannel.show(); });
                    resolve(true);
                } else {
                    this.outputChannel.appendLine(`\n❌ Build failed (exit code: ${code})\n`);
                    vscode.window.showErrorMessage(
                        `❌ AeroLang build failed (code ${code})`,
                        'Show Output'
                    ).then(sel => { if (sel) this.outputChannel.show(); });
                    resolve(false);
                }
            });

            proc.on('error', (err) => {
                this.outputChannel.appendLine(
                    `\n❌ Could not start build: ${err.message}\n` +
                    `   Make sure 'aero-build' is at: ${aeroBuild}\n`
                );
                vscode.window.showErrorMessage(
                    `Could not start AeroLang build: ${err.message}`
                );
                resolve(false);
            });

            token.onCancellationRequested(() => {
                proc.kill('SIGINT');
                this.outputChannel.appendLine('\n⚠️  Build cancelled by user.\n');
                resolve(false);
            });
        }));
    }

    /**
     * Build the project then install the resulting APK on the selected device.
     */
    async deployToDevice() {
        const success = await this.buildProject();
        if (!success) return;

        // Look for the APK in the standard output location
        const workspaceFolder = vscode.workspace.workspaceFolders?.[0];
        if (!workspaceFolder) return;

        const outputDir = path.join(workspaceFolder.uri.fsPath, 'build', 'output');
        let apkFile = '';

        if (fs.existsSync(outputDir)) {
            const apks = fs.readdirSync(outputDir).filter(f => f.endsWith('.apk'));
            if (apks.length > 0) {
                apkFile = path.join(outputDir, apks[apks.length - 1]);
            }
        }

        if (!apkFile) {
            vscode.window.showWarningMessage(
                'Build succeeded but no APK found. ' +
                'Make sure --ndk and --sdk are configured in AeroLang settings.'
            );
            return;
        }

        // Delegate install to DeviceManager
        const DeviceManager = require('./DeviceManager');
        if (DeviceManager && typeof DeviceManager.installApkFile === 'function') {
            await DeviceManager.installApkFile(apkFile);
        } else {
            this.outputChannel.appendLine(`\nAPK ready to install: ${apkFile}`);
            vscode.window.showInformationMessage(`APK ready: ${path.basename(apkFile)}`);
        }
    }
}

module.exports = BuildManager;
