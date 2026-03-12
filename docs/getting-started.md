# AeroLang - Getting Started Guide

## Installation

### Prerequisites
- CMake 3.20 or higher
- C++17-compatible compiler (GCC, Clang, or MSVC)
- Android NDK (for building Android apps)

### Building AeroLang

```bash
# Clone or navigate to AeroLang directory
cd /path/to/AeroLang

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build
cmake --build .

# Run tests
ctest
```

This will create the `aeroc` compiler in the `build` directory.

## Your First AeroLang Program

### 1. Create a new file `Hello.aero`

```aerolang
activity MainActivity {
    var message: String = "Hello, AeroLang!"
    
    fun onCreate() {
        showToast(message)
    }
}
```

### 2. Compile to C++

```bash
./aeroc Hello.aero -o MainActivity.cpp
```

This generates `MainActivity.cpp` with native C++ code.

### 3. Build for Android

To create an Android app, you'll need:
- An Android project with NDK support
- The AeroLang runtime library linked
- JNI bridge configuration

## Language Basics

### Variables

```aerolang
var mutableValue: Int = 42        // Can be changed
val immutableValue: String = "Hi" // Cannot be changed
```

### Functions

```aerolang
fun add(a: Int, b: Int): Int {
    return a + b
}

fun greet(name: String) {
    showToast("Hello, " + name)
}
```

### Activities

```aerolang
activity MyActivity {
    var counter: Int = 0
    
    fun onCreate() {
        // Called when activity is created
    }
    
    fun onButtonClick() {
        counter = counter + 1
    }
}
```

### Classes

```aerolang
class User {
    var name: String
    var age: Int
    
    fun introduce(): String {
        return "I'm " + name
    }
}
```

## Type System

- `Int` - 32-bit integer
- `Long` - 64-bit integer
- `Float` - 32-bit floating point
- `Double` - 64-bit floating point
- `Boolean` - true/false
- `String` - UTF-8 text

## Next Steps

- Read the [Language Specification](language-spec.md)
- Explore [Examples](../examples/)
- Learn about [Android Integration](#)

## Project Structure

```
MyAeroApp/
├── src/
│   └── MainActivity.aero
├── build/
│   └── MainActivity.cpp (generated)
└── android/
    └── (Android project files)
```

## Advantages

✅ **Native Performance** - No JVM overhead  
✅ **Modern Syntax** - Familiar to Kotlin developers  
✅ **Lower Memory** - Direct C++ compilation  
✅ **Android First** - Built-in Android components  

## Current Limitations

⚠️ This is an early version (0.1.0). Features in development:
- Full Android API coverage
- IDE tooling (VS Code extension)
- Debugging support
- Standard library expansion
