# AeroLang

**A modern, high-performance programming language for native Android development**

AeroLang is a Kotlin-inspired programming language that compiles directly to C++ for building maximum-performance Android applications. It completely bypasses the traditional Java Virtual Machine (JVM) overhead, allowing you to write beautiful, high-level code that executes at native C++ hardware speeds.

## ✨ Features

- 🚀 **Native Performance**: Compiles to C++ and runs directly on the Android NDK.
- ⚡ **JNICache Optimization**: Built-in Meyer's Singleton mechanism caches JNI `jclass` and `jmethodID` pointers at startup, meaning AeroLang UI rendering and method calls are **substantially faster** than standard Java/Kotlin applications.
- 📱 **Android-First Design**: Built-in activities, UI layouts, and deep Android API integrations.
- 🎨 **Modern IDE Support**: Fully-featured VS Code configuration with Auto-Complete IntelliSense, inline Markdown Hover documentation, and automated APK building.
- 🎯 **Type Safe**: Strong static typing with intelligent type inference.
- 🧠 **Low Memory Footprint**: No JVM Garbage Collection stutters or bloat.

## 📦 What's Included

The AeroLang Alpha Release comes with a massive, batteries-included standard library:

- **Hardware Sensors**: Poll the `Accelerometer`, `Gyroscope`, `AmbientTemperature`, `Proximity`, and `MagneticField` natively via `SensorManager`.
- **GPS & Network Location**: Tap into the device's location instantly with `LocationManager`.
- **Multimedia Engine**: Built-in native wrappers for `Camera`, `VideoView`, and `MediaPlayer`.
- **Crypto & Encoding**: Securely hash data with `Crypto.md5()` / `Crypto.sha256()` or parse payloads via `Base64` and `URL`.
- **Core Library**: `Timer`, `Console`, `UUID`, `File` I/O, `List<T>`, `HashMap<K,V>`, and `Set<T>`.

## 💻 Quick Example

```aerolang
activity SensorDashboard {
    var sensors: SensorManager
    var uiLayout: LinearLayout
    
    fun onCreate() {
        sensors = SensorManager()
        uiLayout = LinearLayout(Orientation::VERTICAL)
        
        val tempLabel = TextView("Temperature: " + sensors.getAmbientTemperature())
        val accelLabel = TextView("Accel X: " + sensors.getAccelerometerX())
        
        uiLayout.addView(tempLabel)
        uiLayout.addView(accelLabel)
        
        Log.i("AeroLang", "Sensor Dashboard initialized bypassing JVM overhead!")
        
        setContentView(uiLayout)
    }
}
```

## 🛠️ Installation

### 1. The VS Code Extension (Recommended)
You can write, compile, and deploy AeroLang applications directly from VS Code. 

1. Navigate to `/vscode-extension` and run `vsce package` (or grab the pre-compiled `.vsix` file).
2. Install the `.vsix` file into Visual Studio Code.
3. Open VS Code Settings (`Ctrl + ,`), search for `AeroLang`, and configure your absolute **Android SDK Path** and **Android NDK Path**.
4. Press `Ctrl + Shift + P` and type `AeroLang: Build Android APK` to compile your project! 

### 2. Manual CLI Compilation

```bash
# Build the compiler
mkdir build && cd build
cmake ..
cmake --build .

# Compile a specific AeroLang file to C++
./aeroc MyApp.aero -o output.cpp

# Compile an entire directory into a cryptographically signed App Release APK
./tools/aero-build --release /path/to/project --ndk /path/to/android/ndk
```

## 🏗️ Architecture

```
AeroLang Source (.aero)
       ↓
    Lexer (Tokenization)
       ↓
    Parser (Syntax Analysis)
       ↓
    AST (Abstract Syntax Tree)
       ↓
 Semantic Analyzer (Evaluation)
       ↓
 Code Generator (C++ Transpiler)
       ↓
 C++ Native Code + JNICache
       ↓
  Android NDK Build / Gradle
       ↓
    Blazing Fast Native APK
```

## 🏆 Why AeroLang?

### Traditional Android Development
- **Java/Kotlin** → JVM bytecode → Dalvik/ART VM → Native code
- Plagued by JVM overhead, Garbage Collection pauses, and bloated APK sizes.

### The AeroLang Approach
- **AeroLang** → C++ → Native ARM binaries
- Direct hardware execution, drastically lower memory footprint, instantaneous UI updates via the Meyer's Singleton JNI caching mechanism.

## 🤝 Contributing
AeroLang is an experimental project pushing the limits of alternative Android development environments. Contributions are more than welcome!

## 📜 License
MIT License - See LICENSE file for details
