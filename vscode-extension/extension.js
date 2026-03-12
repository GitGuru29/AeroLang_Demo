const vscode = require('vscode');
const { exec, spawn } = require('child_process');
const path = require('path');
const fs = require('fs');
const DeviceManager = require('./src/DeviceManager');
const LogcatViewer = require('./src/LogcatViewer');
const BuildManager = require('./src/BuildManager');
const EmbeddedScreenMirror = require('./src/ScreenMirrorViewer');
const AeroLangIntelliSenseProvider = require('./src/IntelliSenseProvider');
const AeroLangHoverProvider = require('./src/HoverProvider');
const AeroDebugConfigProvider = require('./src/DebugConfigProvider');

let deviceManager;
let logcatViewer;
let buildManager;
let screenMirrorViewer;

function activate(context) {
    console.log('AeroLang extension v2.0 is now active!');

    // Initialize managers
    deviceManager = new DeviceManager(context);
    logcatViewer = new LogcatViewer(context);
    buildManager = new BuildManager(context);
    screenMirrorViewer = new EmbeddedScreenMirror(context);

    // Compile command
    let compileCmd = vscode.commands.registerCommand('aerolang.compile', async () => {
        const editor = vscode.window.activeTextEditor;
        if (!editor) {
            vscode.window.showErrorMessage('No active editor');
            return;
        }

        const filePath = editor.document.uri.fsPath;
        if (!filePath.endsWith('.aero')) {
            vscode.window.showErrorMessage('Not an AeroLang file');
            return;
        }

        await editor.document.save();

        const outputPath = filePath.replace('.aero', '.cpp');

        vscode.window.withProgress({
            location: vscode.ProgressLocation.Notification,
            title: "Compiling AeroLang...",
            cancellable: false
        }, async (progress) => {
            return new Promise((resolve) => {
                exec(`aeroc "${filePath}" -o "${outputPath}" --source-map`, (error, stdout, stderr) => {
                    if (error) {
                        vscode.window.showErrorMessage(`Compilation failed: ${stderr}`);
                        resolve();
                        return;
                    }
                    vscode.window.showInformationMessage(`✅ Compiled to ${path.basename(outputPath)}`);
                    vscode.workspace.openTextDocument(outputPath).then(doc => {
                        vscode.window.showTextDocument(doc, vscode.ViewColumn.Beside);
                    });
                    resolve();
                });
            });
        });
    });

    // Build project command
    let buildCmd = vscode.commands.registerCommand('aerolang.build', async () => {
        await buildManager.buildProject();
    });

    // Deploy to device command
    let deployCmd = vscode.commands.registerCommand('aerolang.deploy', async () => {
        await buildManager.deployToDevice();
    });

    // Device manager commands
    let refreshDevicesCmd = vscode.commands.registerCommand('aerolang.refreshDevices', async () => {
        await deviceManager.refreshDevices();
    });

    let selectDeviceCmd = vscode.commands.registerCommand('aerolang.selectDevice', async () => {
        await deviceManager.selectDevice();
    });

    let launchEmulatorCmd = vscode.commands.registerCommand('aerolang.launchEmulator', async () => {
        await deviceManager.launchEmulator();
    });

    let installApkCmd = vscode.commands.registerCommand('aerolang.installApk', async () => {
        await deviceManager.installApk();
    });

    // Logcat viewer commands
    let showLogcatCmd = vscode.commands.registerCommand('aerolang.showLogcat', async () => {
        await logcatViewer.show();
    });

    let clearLogcatCmd = vscode.commands.registerCommand('aerolang.clearLogcat', async () => {
        await logcatViewer.clear();
    });

    let showDeviceScreenCmd = vscode.commands.registerCommand('aerolang.showDeviceScreen', async (device) => {
        if (!device) {
            const selectedDevice = deviceManager.getSelectedDevice();
            if (!selectedDevice) {
                vscode.window.showErrorMessage('No device selected');
                return;
            }
            device = selectedDevice;
        }
        await screenMirrorViewer.showDeviceScreen(device);
    });

    // Register all commands
    context.subscriptions.push(
        compileCmd,
        buildCmd,
        deployCmd,
        refreshDevicesCmd,
        selectDeviceCmd,
        launchEmulatorCmd,
        installApkCmd,
        showLogcatCmd,
        clearLogcatCmd,
        showDeviceScreenCmd
    );

    // Register tree data providers
    deviceManager.registerTreeDataProvider();

    // Register IntelliSense
    const provider = new AeroLangIntelliSenseProvider();
    const hoverProvider = new AeroLangHoverProvider();
    const selector = { scheme: 'file', language: 'aerolang' };

    context.subscriptions.push(
        vscode.languages.registerCompletionItemProvider(selector, provider, '.'),
        vscode.languages.registerHoverProvider(selector, hoverProvider)
    );

    // ── Debug Adapter ─────────────────────────────────────────────────────────
    const debugProvider = new AeroDebugConfigProvider();
    context.subscriptions.push(
        vscode.debug.registerDebugConfigurationProvider('aerolang', debugProvider)
    );

    // aerolang.debugAero — quick-launch debug session for the active .aero file
    let debugCmd = vscode.commands.registerCommand('aerolang.debugAero', async () => {
        const editor = vscode.window.activeTextEditor;
        if (!editor || !editor.document.fileName.endsWith('.aero')) {
            vscode.window.showErrorMessage('Open a .aero file first.');
            return;
        }
        await editor.document.save();
        // Ensure a source map is up-to-date before debugging
        const filePath = editor.document.uri.fsPath;
        const outputPath = filePath.replace('.aero', '.cpp');
        exec(`aeroc "${filePath}" -o "${outputPath}" --source-map`, () => {
            vscode.debug.startDebugging(undefined, {
                type: 'aerolang',
                request: 'launch',
                name: 'Debug AeroLang App',
                program: filePath,
                workspaceRoot: vscode.workspace.workspaceFolders?.[0]?.uri.fsPath || path.dirname(filePath),
            });
        });
    });
    context.subscriptions.push(debugCmd);

    // ── Gradle Init ───────────────────────────────────────────────────────────
    let initGradleCmd = vscode.commands.registerCommand('aerolang.initGradle', async () => {
        const workspaceRoot = vscode.workspace.workspaceFolders?.[0]?.uri.fsPath;
        if (!workspaceRoot) {
            vscode.window.showErrorMessage('Open a folder/workspace first.');
            return;
        }
        const aeroGradle = path.join(__dirname, '..', 'tools', 'aero-gradle');
        vscode.window.withProgress({
            location: vscode.ProgressLocation.Notification,
            title: 'AeroLang: Initializing Gradle files…',
            cancellable: false
        }, () => new Promise((resolve) => {
            exec(`"${aeroGradle}" init "${workspaceRoot}"`, (err, stdout, stderr) => {
                if (err) {
                    vscode.window.showErrorMessage(`Gradle init failed: ${stderr || err.message}`);
                } else {
                    vscode.window.showInformationMessage('Gradle files initialized! Run aero-build to compile.');
                }
                resolve();
            });
        }));
    });
    context.subscriptions.push(initGradleCmd);

    // Show welcome message
    vscode.window.showInformationMessage('AeroLang v2.0 loaded! IntelliSense and Device Manager ready.');
}

function deactivate() {
    if (logcatViewer) {
        logcatViewer.dispose();
    }
    if (screenMirrorViewer) {
        screenMirrorViewer.dispose();
    }
}

module.exports = {
    activate,
    deactivate
};
