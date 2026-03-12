# Embedded Screen Mirroring - Setup Guide

AeroLang VS Code extension now has **built-in screen mirroring** without external dependencies!

## ✨ Features

- **No external tools** - Uses ADB directly
- **Embedded in VS Code** - Native webview display
- **Touch support** - Click to interact with device
- **Real-time streaming** - ~15 FPS screen capture
- **Keyboard shortcuts** - Back, Home, Recent apps

## 📦 Installation

### 1. Install Node Dependencies

```bash
cd /run/media/msfvenom/28aa095f-4b10-4a14-8ba9-4f2570fb6ce2/AeroLang/vscode-extension
npm install ws
```

### 2. Reload Extension

```bash
# Copy to VS Code extensions
cp -r . ~/.vscode/extensions/aerolang-2.0.0/

# Reload VS Code
code --reload
```

## 🎮 Usage

1. **Connect Device**
   - USB: Plug in your Android phone
   - WiFi: `adb connect <device-ip>:5555`

2. **Start Mirroring**
   - Open AeroLang sidebar (🚀 icon)
   - Click on device name under "Connected Devices"
   - Screen appears in VS Code!

3. **Control Device**
   - **Tap**: Click onscreen
   - **Back**: Click "Back" button or `Esc`
   - **Home**: Click "Home" button
   - **Recent Apps**: Click "Apps" button
   - **Rotate**: Click "Rotate" button

## 🔧 How It Works

```
┌─────────────┐
│  VS Code    │
│  Webview    │ ← Canvas displays frames
└──────┬──────┘
       │ WebSocket
       ↓
┌──────────────┐
│   Daemon     │
│  (Node.js)   │ ← Captures + streams
└──────┬───────┘
       │ ADB
       ↓
┌──────────────┐
│ Android      │
│ Device       │
└──────────────┘
```

1. **Daemon captures** frames via `adb exec-out screencap`
2. **WebSocket streams** frames as base64 PNG
3. **Canvas renders** each frame in webview
4. **Touch events** sent back via `adb shell input tap`

## ⚡ Performance

- **Frame Rate**: ~15 FPS (adjustable)
- **Latency**: ~200-500ms (depends on USB/WiFi)
- **CPU Usage**: Low (screenshot-based, not video)

## 🐛 Troubleshooting

### "Connection failed"
- Check ADB is installed: `adb version`
- Check device is connected: `adb devices`

### Low frame rate
Edit `ScreenMirrorDaemon.js`:
```javascript
this.frameRate = 30; // Increase from 15 to 30
```

### Touch not working
- Enable USB debugging on device
- Allow USB debugging from computer

## 🆚 vs scrcpy

| Feature | Embedded | scrcpy |
|---------|----------|--------|
| Installation | Built-in ✅ | External ❌ |
| Frame Rate | ~15 FPS | 60+ FPS |
| Latency | Medium | Low |
| CPU Usage | Low | Medium |
| Integration | VS Code native | Separate window |

**Use embedded for**: Quick testing, integrated workflow  
**Use scrcpy for**: High-performance gaming, recording

## 🔄 Switching Between Methods

Both screen mirroring methods are available:

- **Embedded** (default): Click device in sidebar
- **scrcpy**: Command Palette → "Show Device Screen (scrcpy)"

## 📝 Notes

- Frame rate optimized for testing apps, not gaming
- Uses PNG screenshots (not video stream)
- Works over WiFi and USB
- No additional permissions needed

---

**Enjoy embedded Android screen mirroring in VS Code!** 🚀
