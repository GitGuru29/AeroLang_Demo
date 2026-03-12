const vscode = require('vscode');
const { exec } = require('child_process');
const path = require('path');
const fs = require('fs');
const util = require('util');
const execPromise = util.promisify(exec);

class DeviceManager {
    constructor(context) {
        this.context = context;
        this.devices = [];
        this.emulators = [];
        this.selectedDevice = null;
        this._onDidChangeTreeData = new vscode.EventEmitter();
        this.onDidChangeTreeData = this._onDidChangeTreeData.event;
    }

    registerTreeDataProvider() {
        vscode.window.registerTreeDataProvider('aerolangDevices', this);
    }

    refresh() {
        this._onDidChangeTreeData.fire();
    }

    getTreeItem(element) {
        return element;
    }

    async getChildren(element) {
        if (!element) {
            // Root level - show categories
            return [
                new DeviceCategory('Connected Devices', 'devices'),
                new DeviceCategory('Emulators', 'emulators')
            ];
        }

        if (element.category === 'devices') {
            await this.refreshDevices();
            return this.devices.map(d => new DeviceItem(d));
        }

        if (element.category === 'emulators') {
            await this.refreshEmulators();
            return this.emulators.map(e => new EmulatorItem(e));
        }

        return [];
    }

    async refreshDevices() {
        try {
            const { stdout } = await execPromise('adb devices -l');
            const lines = stdout.split('\n').slice(1); // Skip header

            this.devices = lines
                .filter(line => line.trim() && !line.includes('List of devices'))
                .map(line => {
                    const parts = line.trim().split(/\s+/);
                    const serial = parts[0];
                    const status = parts[1];
                    const model = (line.match(/model:(\S+)/) || [])[1] || 'Unknown';

                    return { serial, status, model };
                })
                .filter(d => d.status === 'device');

            this.refresh();
            return this.devices;
        } catch (error) {
            vscode.window.showErrorMessage('ADB not found. Please install Android SDK Platform Tools.');
            return [];
        }
    }

    async refreshEmulators() {
        try {
            const { stdout } = await execPromise('emulator -list-avds');
            this.emulators = stdout.trim().split('\n').filter(e => e.trim());
            this.refresh();
            return this.emulators;
        } catch (error) {
            console.log('Emulator command not available');
            return [];
        }
    }

    async selectDevice() {
        await this.refreshDevices();

        if (this.devices.length === 0) {
            vscode.window.showWarningMessage('No devices connected');
            return;
        }

        const items = this.devices.map(d => ({
            label: d.model,
            description: d.serial,
            device: d
        }));

        const selected = await vscode.window.showQuickPick(items, {
            placeHolder: 'Select target device'
        });

        if (selected) {
            this.selectedDevice = selected.device;
            vscode.window.showInformationMessage(`Selected: ${selected.device.model}`);
        }
    }

    async launchEmulator() {
        await this.refreshEmulators();

        if (this.emulators.length === 0) {
            vscode.window.showWarningMessage('No emulators configured. Create one in Android Studio.');
            return;
        }

        const selected = await vscode.window.showQuickPick(this.emulators, {
            placeHolder: 'Select emulator to launch'
        });

        if (selected) {
            vscode.window.showInformationMessage(`Launching ${selected}...`);
            exec(`emulator -avd ${selected} &`, (error) => {
                if (error) {
                    vscode.window.showErrorMessage(`Failed to launch emulator: ${error.message}`);
                } else {
                    vscode.window.showInformationMessage(`Emulator ${selected} started!`);
                }
            });
        }
    }

    async installApk() {
        const device = this.selectedDevice || (await this.getFirstDevice());
        if (!device) {
            vscode.window.showErrorMessage('No device selected');
            return;
        }

        const workspaceFolder = vscode.workspace.workspaceFolders?.[0];
        if (!workspaceFolder) {
            vscode.window.showErrorMessage('No workspace folder open');
            return;
        }

        const apkPath = path.join(workspaceFolder.uri.fsPath, 'build', 'app.apk');

        if (!fs.existsSync(apkPath)) {
            vscode.window.showErrorMessage('APK not found. Build the project first.');
            return;
        }

        vscode.window.withProgress({
            location: vscode.ProgressLocation.Notification,
            title: `Installing APK on ${device.model}...`,
            cancellable: false
        }, async (progress) => {
            return new Promise((resolve) => {
                exec(`adb -s ${device.serial} install -r "${apkPath}"`, (error, stdout) => {
                    if (error) {
                        vscode.window.showErrorMessage(`Installation failed: ${error.message}`);
                    } else {
                        vscode.window.showInformationMessage('✅ APK installed successfully!');
                    }
                    resolve();
                });
            });
        });
    }

    async getFirstDevice() {
        await this.refreshDevices();
        return this.devices[0] || null;
    }

    getSelectedDevice() {
        return this.selectedDevice;
    }
}

class DeviceCategory extends vscode.TreeItem {
    constructor(label, category) {
        super(label, vscode.TreeItemCollapsibleState.Expanded);
        this.category = category;
        this.contextValue = 'category';
    }
}

class DeviceItem extends vscode.TreeItem {
    constructor(device) {
        super(device.model, vscode.TreeItemCollapsibleState.None);
        this.description = device.serial;
        this.tooltip = `${device.model} (${device.serial})\nClick to show screen`;
        this.iconPath = new vscode.ThemeIcon('device-mobile');
        this.contextValue = 'device';
        this.device = device;
        this.command = {
            command: 'aerolang.showDeviceScreen',
            title: 'Show Device Screen',
            arguments: [device]
        };
    }
}

class EmulatorItem extends vscode.TreeItem {
    constructor(emulator) {
        super(emulator, vscode.TreeItemCollapsibleState.None);
        this.tooltip = `Launch ${emulator}`;
        this.iconPath = new vscode.ThemeIcon('vm');
        this.contextValue = 'emulator';
        this.command = {
            command: 'aerolang.launchEmulator',
            title: 'Launch Emulator',
            arguments: [emulator]
        };
    }
}

module.exports = DeviceManager;
