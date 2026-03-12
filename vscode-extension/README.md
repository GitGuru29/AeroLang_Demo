# AeroLang VS Code Extension v2.0

Complete IDE support for AeroLang with device management and deployment capabilities.

## 🎯 Features

### 📝 Language Support
- **Syntax Highlighting** - Full TextMate grammar for `.aero` files
- **Code Completion** - IntelliSense for keywords and symbols
- **Bracket Matching** - Auto-closing pairs and indentation

### 🔨 Build & Compile
- **Compile to C++** - One-click compilation (`aeroc`)
- **Build Android APK** - Complete project build with NDK
- **Build Output Panel** - Real-time build progress

### 📱 Device Management
- **Device Detection** - Automatic ADB device discovery
- **Emulator Support** - Launch AVDs directly from VS Code
- **Device Selection** - Choose target device for deployment
- **Real-time Refresh** - Monitor connected devices

### 🚀 Deployment
- **One-Click Deploy** - Build + Install to device
- **APK Installation** - Direct APK push via ADB
- **Progress Tracking** - Visual deployment feedback

### 📊 Debugging
- **Logcat Viewer** - Real-time device logs
- **Log Filtering** - Color-coded by severity (Error, Warning, Info, Debug)
- **Clear Logs** - Quick logcat cleanup

## 📦 Installation

### Prerequisites
- **VS Code** 1.80.0 or higher
- **AeroLang Compiler** (`aeroc` in PATH)
- **Android SDK Platform Tools** (adb)
- **Android NDK** (optional, for building APKs)
- **Android Emulator** (optional, for AVD support)

### Install Extension

```bash
# Copy to VS Code extensions folder
cp -r vscode-extension ~/.vscode/extensions/aerolang-2.0.0/

# Or create symlink
ln -s $(pwd)/vscode-extension ~/.vscode/extensions/aerolang-2.0.0

# Reload VS Code
code --reload
```

## 🎮 Usage

### Compiling Code

1. Open a `.aero` file
2. Press `Ctrl+Shift+P` (Cmd+Shift+P on Mac)
3. Run: **AeroLang: Compile to C++**
4. Generated `.cpp` file opens side-by-side

### Building Android APK

1. Open AeroLang project folder
2. Run: **AeroLang: Build Android APK**
3. Check build output panel for progress
4. APK generated in `build/app.apk`

### Deploying to Device

1. Connect Android device or launch emulator
2. Open device sidebar (AeroLang icon in Activity Bar)
3. See connected devices
4. Run: **AeroLang: Build & Deploy to Device**
5. App installs automatically

### Managing Devices

**Sidebar View:**
- Click AeroLang icon (🚀) in Activity Bar
- See "Devices & Emulators" panel
- Click refresh icon to detect devices

**Launch Emulator:**
- Expand "Emulators" in device panel
- Click on emulator name to launch

**View Logs:**
- Run: **AeroLang: Show Device Logcat**
- See real-time logs from device
- Color-coded: ❌ Error, ⚠️ Warning, ℹ️ Info, 🔍 Debug

## ⌨️ Commands

| Command | Description |
|---------|-------------|
| `AeroLang: Compile to C++` | Compile current `.aero` file |
| `AeroLang: Build Android APK` | Build entire project |
| `AeroLang: Build & Deploy to Device` | Build + install on device |
| `Select Target Device` | Choose deployment target |
| `Launch Emulator` | Start Android Virtual Device |
| `Install APK on Device` | Push APK to connected device |
| `Show Device Logcat` | Open real-time log viewer |
| `Clear Logcat` | Reset device logs |
| `Refresh Devices` | Re-scan for connected devices |

## 🔧 Configuration

No additional configuration needed! The extension auto-detects:
- AeroLang compiler (`aeroc`)
- Android SDK (`adb`)
- Android emulators (AVD)

## 📁 Project Structure

```
your-project/
├── src/
│   ├── MainActivity.aero
│   └── Calculator.aero
├── build/
│   ├── *.cpp (generated)
│   └── app.apk (built APK)
└── android/
    └── (Android project files)
```

## 🐛 Troubleshooting

### "ADB not found"
Install Android SDK Platform Tools:
```bash
# Ubuntu/Debian
sudo apt install android-tools-adb

# Mac
brew install android-platform-tools
```

### "Compiler not found"
Ensure `aeroc` is in your PATH:
```bash
export PATH=$PATH:/path/to/AeroLang/build
```

### No devices shown
- Check USB debugging enabled on device
- Run `adb devices` to verify connection
- Click refresh button in device panel

## 🎯 What's New in v2.0

- ✅ Device Manager sidebar
- ✅ ADB integration
- ✅ Emulator launcher
- ✅ Real-time logcat viewer
- ✅ One-click deployment
- ✅ Build output panel
- ✅ APK installation

## 📜 License

MIT License - See LICENSE file

## 🤝 Contributing

See CONTRIBUTING.md in the main AeroLang repository.

## 🔗 Links

- **GitHub**: https://github.com/GitGuru29/AeroLang
- **Documentation**: https://github.com/GitGuru29/AeroLang/tree/main/docs
- **Issues**: https://github.com/GitGuru29/AeroLang/issues
