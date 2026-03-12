const { spawn } = require('child_process');
const http = require('http');
const WebSocket = require('ws');
const { Transform } = require('stream');

class H264ScreenMirrorDaemon {
    constructor() {
        this.server = null;
        this.wss = null;
        this.screenrecordProcess = null;
        this.ffmpegProcess = null;
        this.isRunning = false;
        this.port = 8765;
        this.deviceSerial = null;
    }

    async start(deviceSerial) {
        this.deviceSerial = deviceSerial;

        // Create HTTP + WebSocket server
        this.server = http.createServer();
        this.wss = new WebSocket.Server({ server: this.server });

        this.wss.on('connection', (ws) => {
            console.log('H.264 stream client connected');

            ws.on('message', (message) => {
                this.handleTouchEvent(JSON.parse(message));
            });

            ws.on('close', () => {
                console.log('H.264 stream client disconnected');
            });

            // Start streaming to this client
            if (!this.screenrecordProcess) {
                this.startH264Stream(ws);
            }
        });

        // Start server
        return new Promise((resolve, reject) => {
            this.server.listen(this.port, '127.0.0.1', () => {
                console.log(`H.264 screen mirror daemon running on ws://localhost:${this.port}`);
                this.isRunning = true;
                resolve(this.port);
            });

            this.server.on('error', (err) => {
                reject(err);
            });
        });
    }

    startH264Stream(ws) {
        console.log('Starting H.264 hardware encoding stream...');

        // Start screenrecord with H.264 output
        // --bit-rate 2000000 = 2 Mbps (good quality)
        // --size 720x1280 = Scaled down for performance (adjust based on device)
        // --output-format=h264 = Raw H.264 stream
        const screenrecordArgs = [
            '-s', this.deviceSerial,
            'shell',
            'screenrecord',
            '--output-format=h264',
            '--bit-rate', '2000000',
            '--time-limit', '180', // 3 minutes, will auto-restart
            '-'
        ];

        this.screenrecordProcess = spawn('adb', screenrecordArgs);

        // Use ffmpeg to decode H.264 to JPEG frames
        const ffmpegArgs = [
            '-f', 'h264',           // Input format
            '-i', 'pipe:0',         // Input from stdin
            '-vf', 'scale=720:-1',  // Scale to 720px width
            '-q:v', '3',            // JPEG quality (2-5 is good)
            '-f', 'image2pipe',     // Output format
            '-vcodec', 'mjpeg',     // MJPEG codec
            '-r', '30',             // 30 FPS
            'pipe:1'                // Output to stdout
        ];

        this.ffmpegProcess = spawn('ffmpeg', ffmpegArgs);

        // Pipe screenrecord output to ffmpeg input
        this.screenrecordProcess.stdout.pipe(this.ffmpegProcess.stdin);

        // Handle ffmpeg output (JPEG frames)
        let frameBuffer = Buffer.alloc(0);
        const SOI = Buffer.from([0xFF, 0xD8]); // JPEG Start Of Image
        const EOI = Buffer.from([0xFF, 0xD9]); // JPEG End Of Image

        this.ffmpegProcess.stdout.on('data', (chunk) => {
            frameBuffer = Buffer.concat([frameBuffer, chunk]);

            // Look for complete JPEG frames
            let start = 0;
            while (true) {
                const startIdx = frameBuffer.indexOf(SOI, start);
                if (startIdx === -1) break;

                const endIdx = frameBuffer.indexOf(EOI, startIdx + 2);
                if (endIdx === -1) break;

                // Extract complete JPEG frame
                const jpegFrame = frameBuffer.slice(startIdx, endIdx + 2);
                const base64Frame = jpegFrame.toString('base64');

                // Broadcast to all connected clients
                this.wss.clients.forEach((client) => {
                    if (client.readyState === WebSocket.OPEN) {
                        client.send(JSON.stringify({
                            type: 'frame',
                            data: base64Frame
                        }));
                    }
                });

                start = endIdx + 2;
            }

            // Keep remaining incomplete data
            if (start > 0) {
                frameBuffer = frameBuffer.slice(start);
            }
        });

        this.screenrecordProcess.stderr.on('data', (data) => {
            console.log('screenrecord:', data.toString());
        });

        this.ffmpegProcess.stderr.on('data', (data) => {
            // Suppress ffmpeg verbose output
            const msg = data.toString();
            if (msg.includes('error') || msg.includes('Error')) {
                console.error('ffmpeg error:', msg);
            }
        });

        this.screenrecordProcess.on('close', (code) => {
            console.log(`screenrecord process exited with code ${code}`);
            if (this.isRunning) {
                // Auto-restart after delay
                console.log('Restarting screenrecord in 1 second...');
                setTimeout(() => {
                    if (this.isRunning) {
                        this.startH264Stream(ws);
                    }
                }, 1000);
            }
        });

        this.ffmpegProcess.on('close', (code) => {
            console.log(`ffmpeg process exited with code ${code}`);
        });
    }

    handleTouchEvent(event) {
        if (!this.deviceSerial) return;

        const { type, x, y } = event;
        let adbCommand;
        let args;

        switch (type) {
            case 'tap':
                args = ['-s', this.deviceSerial, 'shell', 'input', 'tap', x.toString(), y.toString()];
                break;
            case 'swipe':
                const { x2, y2, duration } = event;
                args = ['-s', this.deviceSerial, 'shell', 'input', 'swipe',
                    x.toString(), y.toString(), x2.toString(), y2.toString(), (duration || 100).toString()];
                break;
            case 'text':
                const { text } = event;
                args = ['-s', this.deviceSerial, 'shell', 'input', 'text', text.replace(/\s/g, '%s')];
                break;
            case 'keyevent':
                const { keycode } = event;
                args = ['-s', this.deviceSerial, 'shell', 'input', 'keyevent', keycode.toString()];
                break;
            default:
                return;
        }

        // Execute touch command asynchronously
        const touchProcess = spawn('adb', args);
        touchProcess.on('error', (error) => {
            console.error('Touch event error:', error.message);
        });
    }

    stop() {
        this.isRunning = false;

        if (this.screenrecordProcess) {
            this.screenrecordProcess.kill('SIGTERM');
            this.screenrecordProcess = null;
        }

        if (this.ffmpegProcess) {
            this.ffmpegProcess.kill('SIGTERM');
            this.ffmpegProcess = null;
        }

        if (this.wss) {
            this.wss.close();
            this.wss = null;
        }

        if (this.server) {
            this.server.close();
            this.server = null;
        }
    }

    getPort() {
        return this.port;
    }
}

module.exports = H264ScreenMirrorDaemon;
