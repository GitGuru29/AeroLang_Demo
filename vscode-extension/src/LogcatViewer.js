const vscode = require('vscode');
const { spawn } = require('child_process');

class LogcatViewer {
    constructor(context) {
        this.context = context;
        this.outputChannel = vscode.window.createOutputChannel('AeroLang Logcat');
        this.logcatProcess = null;
    }

    async show() {
        // Get device serial
        const { exec } = require('child_process');
        const util = require('util');
        const execPromise = util.promisify(exec);

        try {
            const { stdout } = await execPromise('adb devices');
            const lines = stdout.split('\n').slice(1);
            const devices = lines
                .filter(line => line.trim() && line.includes('device'))
                .map(line => line.trim().split('\t')[0]);

            if (devices.length === 0) {
                vscode.window.showWarningMessage('No devices connected');
                return;
            }

            const device = devices[0];
            this.startLogcat(device);
            this.outputChannel.show();

        } catch (error) {
            vscode.window.showErrorMessage('Failed to start logcat: ' + error.message);
        }
    }

    startLogcat(device) {
        if (this.logcatProcess) {
            this.logcatProcess.kill();
        }

        this.outputChannel.clear();
        this.outputChannel.appendLine(`=== Logcat started for ${device} ===\n`);

        this.logcatProcess = spawn('adb', ['-s', device, 'logcat', '-v', 'time']);

        this.logcatProcess.stdout.on('data', (data) => {
            const lines = data.toString().split('\n');
            lines.forEach(line => {
                if (line.trim()) {
                    // Color code based on log level
                    if (line.includes(' E ')) {
                        this.outputChannel.appendLine(`❌ ${line}`);
                    } else if (line.includes(' W ')) {
                        this.outputChannel.appendLine(`⚠️  ${line}`);
                    } else if (line.includes(' I ')) {
                        this.outputChannel.appendLine(`ℹ️  ${line}`);
                    } else if (line.includes(' D ')) {
                        this.outputChannel.appendLine(`🔍 ${line}`);
                    } else {
                        this.outputChannel.appendLine(line);
                    }
                }
            });
        });

        this.logcatProcess.stderr.on('data', (data) => {
            this.outputChannel.appendLine(`ERROR: ${data}`);
        });

        this.logcatProcess.on('close', (code) => {
            this.outputChannel.appendLine(`\n=== Logcat stopped (exit code: ${code}) ===`);
        });
    }

    async clear() {
        const { exec } = require('child_process');
        exec('adb logcat -c', (error) => {
            if (error) {
                vscode.window.showErrorMessage('Failed to clear logcat');
            } else {
                this.outputChannel.clear();
                vscode.window.showInformationMessage('Logcat cleared');
            }
        });
    }

    dispose() {
        if (this.logcatProcess) {
            this.logcatProcess.kill();
        }
        this.outputChannel.dispose();
    }
}

module.exports = LogcatViewer;
