const vscode = require('vscode');
const H264ScreenMirrorDaemon = require('./ScreenMirrorDaemon');

class EmbeddedScreenMirror {
    constructor(context) {
        this.context = context;
        this.panels = new Map();
        this.daemons = new Map();
    }

    async showDeviceScreen(device) {
        const deviceSerial = device.serial || device;
        const deviceModel = device.model || 'Android Device';

        if (this.panels.has(deviceSerial)) {
            this.panels.get(deviceSerial).reveal();
            return;
        }

        const daemon = new H264ScreenMirrorDaemon();

        try {
            const port = await daemon.start(deviceSerial);

            const panel = vscode.window.createWebviewPanel(
                'embeddedMirror',
                `📱 ${deviceModel}`,
                vscode.ViewColumn.Two,
                {
                    enableScripts: true,
                    retainContextWhenHidden: true,
                    localResourceRoots: []
                }
            );

            this.panels.set(deviceSerial, panel);
            this.daemons.set(deviceSerial, daemon);

            panel.webview.html = this.getWebviewContent(deviceModel, deviceSerial, port);

            panel.webview.onDidReceiveMessage(
                message => {
                    this.handleWebviewMessage(message, deviceSerial);
                },
                undefined,
                this.context.subscriptions
            );

            panel.onDidDispose(() => {
                this.stopMirror(deviceSerial);
            });

            vscode.window.showInformationMessage(
                `🎥 H.264 hardware streaming started (30 FPS, no device lag!)`
            );

        } catch (error) {
            vscode.window.showErrorMessage(`Failed to start screen mirror: ${error.message}`);
            daemon.stop();
        }
    }

    handleWebviewMessage(message, deviceSerial) {
        const daemon = this.daemons.get(deviceSerial);
        if (!daemon) return;

        switch (message.command) {
            case 'rotate':
                this.sendKeyEvent(deviceSerial, 82);
                break;
            case 'back':
                this.sendKeyEvent(deviceSerial, 4);
                break;
            case 'home':
                this.sendKeyEvent(deviceSerial, 3);
                break;
            case 'recent':
                this.sendKeyEvent(deviceSerial, 187);
                break;
        }
    }

    sendKeyEvent(deviceSerial, keycode) {
        const { exec } = require('child_process');
        exec(`adb -s ${deviceSerial} shell input keyevent ${keycode}`, { timeout: 100 });
    }

    stopMirror(deviceSerial) {
        const daemon = this.daemons.get(deviceSerial);
        if (daemon) {
            daemon.stop();
            this.daemons.delete(deviceSerial);
        }
        this.panels.delete(deviceSerial);
    }

    getWebviewContent(deviceModel, deviceSerial, port) {
        return `<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Device Screen - ${deviceModel}</title>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        
        body {
            background: #1e1e1e;
            color: #fff;
            font-family: system-ui, -apple-system, sans-serif;
            overflow: hidden;
            display: flex;
            flex-direction: column;
            height: 100vh;
        }
        
        .toolbar {
            background: #252526;
            padding: 12px 20px;
            display: flex;
            align-items: center;
            justify-content: space-between;
            border-bottom: 1px solid #3e3e42;
            flex-shrink: 0;
        }
        
        .device-info {
            display: flex;
            align-items: center;
            gap: 12px;
        }
        
        .status-dot {
            width: 8px;
            height: 8px;
            background: #4ec9b0;
            border-radius: 50%;
            animation: pulse 2s infinite;
        }
        
        @keyframes pulse {
            0%, 100% { opacity: 1; }
            50% { opacity: 0.3; }
        }
        
        .controls {
            display: flex;
            gap: 8px;
        }
        
        button {
            background: #2d2d30;
            border: 1px solid #3e3e42;
            color: #ccc;
            padding: 8px 14px;
            border-radius: 4px;
            cursor: pointer;
            font-size: 13px;
            transition: all 0.2s;
        }
        
        button:hover {
            background: #3e3e42;
            border-color: #4ec9b0;
            color: #fff;
        }
        
        button:active {
            transform: scale(0.95);
        }
        
        .screen-container {
            flex: 1;
            display: flex;
            align-items: center;
            justify-content: center;
            background: #000;
            position: relative;
            min-height: 0;
        }
        
        #screen-wrapper {
            width: 100%;
            height: 100%;
            display: flex;
            align-items: center;
            justify-content: center;
            padding: 20px;
        }
        
        canvas {
            max-width: 100%;
            max-height: 100%;
            box-shadow: 0 8px 32px rgba(0,0,0,0.6);
            cursor: crosshair;
            border: 2px solid #3e3e42;
            border-radius: 4px;
        }
        
        .loading {
            position: absolute;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            text-align: center;
            z-index: 10;
        }
        
        .spinner {
            border: 3px solid #3e3e42;
            border-top: 3px solid #4ec9b0;
            border-radius: 50%;
            width: 40px;
            height: 40px;
            animation: spin 1s linear infinite;
            margin: 0 auto 10px;
        }
        
        @keyframes spin {
            0% { transform: rotate(0deg); }
            100% { transform: rotate(360deg); }
        }
        
        .stats {
            font-size: 11px;
            color: #888;
            margin-left: 10px;
        }
        
        .note {
            background: #3e3e42;
            color: #ccc;
            padding: 4px 10px;
            border-radius: 4px;
            font-size: 11px;
        }
    </style>
</head>
<body>
    <div class="toolbar">
        <div class="device-info">
            <div class="status-dot"></div>
            <span>📱 ${deviceModel}</span>
            <span class="stats" id="fps">0 FPS</span>
            <span class="note">🎥 H.264 Hardware Streaming (30 FPS)</span>
        </div>
        <div class="controls">
            <button onclick="sendCommand('back')" title="Back">◀ Back</button>
            <button onclick="sendCommand('home')" title="Home">⌂ Home</button>
            <button onclick="sendCommand('recent')" title="Recent Apps">☰ Apps</button>
        </div>
    </div>
    
    <div class="screen-container">
        <div class="loading" id="loading">
            <div class="spinner"></div>
            <div>Connecting to device...</div>
        </div>
        <div id="screen-wrapper">
            <canvas id="screen"></canvas>
        </div>
    </div>

    <script>
        const vscode = acquireVsCodeApi();
        const canvas = document.getElementById('screen');
        const ctx = canvas.getContext('2d');
        const loading = document.getElementById('loading');
        const fpsDisplay = document.getElementById('fps');
        const wrapper = document.getElementById('screen-wrapper');
        
        let ws = null;
        let frameCount = 0;
        let lastFpsUpdate = Date.now();
        let deviceWidth = 0;
        let deviceHeight = 0;
        let currentImage = null;

        function resizeCanvas() {
            if (!currentImage) return;

            const wrapperWidth = wrapper.clientWidth;
            const wrapperHeight = wrapper.clientHeight;
            const imageAspect = deviceWidth / deviceHeight;
            const wrapperAspect = wrapperWidth / wrapperHeight;

            let displayWidth, displayHeight;

            if (wrapperAspect > imageAspect) {
                displayHeight = wrapperHeight;
                displayWidth = displayHeight * imageAspect;
            } else {
                displayWidth = wrapperWidth;
                displayHeight = displayWidth / imageAspect;
            }

            canvas.style.width = displayWidth + 'px';
            canvas.style.height = displayHeight + 'px';
        }

        function connect() {
            ws = new WebSocket('ws://localhost:${port}');
            
            ws.onopen = () => {
                console.log('Connected');
                loading.style.display = 'none';
            };
            
            ws.onmessage = (event) => {
                const message = JSON.parse(event.data);
                
                if (message.type === 'frame') {
                    displayFrame(message.data);
                    updateFPS();
                }
            };
            
            ws.onerror = (error) => {
                console.error('WebSocket error:', error);
            };
            
            ws.onclose = () => {
                loading.style.display = 'block';
                loading.innerHTML = '<div class="spinner"></div><div>Reconnecting...</div>';
                setTimeout(connect, 2000);
            };
        }

        function displayFrame(base64Data) {
            const img = new Image();
            img.onload = () => {
                currentImage = img;
                
                if (deviceWidth === 0) {
                    deviceWidth = img.width;
                    deviceHeight = img.height;
                    canvas.width = deviceWidth;
                    canvas.height = deviceHeight;
                    resizeCanvas();
                }
                
                ctx.drawImage(img, 0, 0, canvas.width, canvas.height);
            };
            img.src = 'data:image/png;base64,' + base64Data;
        }

        function updateFPS() {
            frameCount++;
            const now = Date.now();
            if (now - lastFpsUpdate >= 1000) {
                fpsDisplay.textContent = frameCount + ' FPS';
                frameCount = 0;
                lastFpsUpdate = now;
            }
        }

        window.addEventListener('resize', () => {
            resizeCanvas();
        });

        canvas.addEventListener('click', (e) => {
            if (!deviceWidth || !deviceHeight) return;
            
            const rect = canvas.getBoundingClientRect();
            const x = Math.floor((e.clientX - rect.left) * (deviceWidth / rect.width));
            const y = Math.floor((e.clientY - rect.top) * (deviceHeight / rect.height));
            
            ws.send(JSON.stringify({
                type: 'tap',
                x: x,
                y: y
            }));
        });

        function sendCommand(command) {
            vscode.postMessage({ command: command });
        }

        connect();
    </script>
</body>
</html>`;
    }

    dispose() {
        for (const [deviceSerial, daemon] of this.daemons) {
            daemon.stop();
        }
        this.daemons.clear();
        this.panels.clear();
    }
}

module.exports = EmbeddedScreenMirror;
