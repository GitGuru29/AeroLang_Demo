# Install scrcpy for Screen Mirroring

AeroLang VS Code extension uses `scrcpy` for device screen mirroring.

## Installation

### Arch Linux
```bash
sudo pacman -S scrcpy
```

### Ubuntu/Debian
```bash
sudo apt install scrcpy
```

### macOS
```bash
brew install scrcpy
```

### Fedora
```bash
sudo dnf install scrcpy
```

### From Source
```bash
# Install dependencies
sudo apt install ffmpeg libsdl2-2.0-0 adb wget \
                 gcc git pkg-config meson ninja-build libsdl2-dev \
                 libavcodec-dev libavdevice-dev libavformat-dev libavutil-dev \
                 libswresample-dev libusb-1.0-0 libusb-1.0-0-dev

# Build scrcpy
git clone https://github.com/Genymobile/scrcpy
cd scrcpy
./install_release.sh
```

## Verify Installation

```bash
scrcpy --version
```

## Usage in VS Code

1. Connect your Android device (USB or WiFi)
2. Open AeroLang sidebar (🚀 icon)
3. Click on any device in "Connected Devices"
4. Screen mirroring window opens automatically!

## Controls

- **Click/Touch**: Click on the mirror window
- **Rotate**: `Ctrl + R`
- **Back**: `Ctrl + B` or Right-click
- **Home**: `Ctrl + H`
- **Power**: `Ctrl + P`
- **Screenshot**: `Ctrl + S`

## Troubleshooting

### "scrcpy not found"
Make sure scrcpy is installed and in your PATH.

### Device not showing
Enable USB debugging on your Android device.

### Screen black
Try unplugging and reconnecting your device.
