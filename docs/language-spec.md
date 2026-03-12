# AeroLang Language Specification

**Version:** 0.1.0  
**Syntax Style:** Kotlin-inspired

## Philosophy

AeroLang is designed to provide Android developers with a familiar, modern syntax while compiling directly to native C++ code. It eliminates JVM overhead while maintaining developer productivity.

## Basic Syntax

### Variables

```aerolang
var name: String = "Hello"
val count: Int = 42
var isActive: Boolean = true
```

### Functions

```aerolang
fun greet(name: String): String {
    return "Hello, $name"
}

fun calculate(a: Int, b: Int): Int {
    return a + b
}
```

### Classes

```aerolang
class User {
    var name: String
    var age: Int
    
    fun greet(): String {
        return "Hello, I'm $name"
    }
}
```

### Activities (Android-specific)

```aerolang
activity MainActivity {
    var userName: String = ""
    
    fun onCreate() {
        setContentView(R.layout.activity_main)
        
        val welcomeText = findViewById<TextView>(R.id.welcome)
        welcomeText.text = "Welcome, $userName!"
        
        val button = findViewById<Button>(R.id.submit)
        button.onClick {
            showMessage("Button clicked!")
        }
    }
}
```

## Type System

### Primitive Types
- `Int` → C++ `int32_t`
- `Long` → C++ `int64_t`
- `Float` → C++ `float`
- `Double` → C++ `double`
- `Boolean` → C++ `bool`
- `String` → Custom `aero::String` (UTF-8 wrapper)

### Nullable Types
```aerolang
var name: String? = null
val age: Int? = null
```

### Collections
```aerolang
val numbers: List<Int> = listOf(1, 2, 3)
val map: Map<String, Int> = mapOf("a" to 1, "b" to 2)
```

## Control Flow

### If/Else
```aerolang
if (condition) {
    // code
} else {
    // code
}
```

### When (switch)
```aerolang
when (value) {
    1 -> print("One")
    2 -> print("Two")
    else -> print("Other")
}
```

### Loops
```aerolang
for (i in 0..10) {
    print(i)
}

while (condition) {
    // code
}
```

## Android UI Components

### Views
```aerolang
TextView {
    id = R.id.title
    text = "Hello World"
    textSize = 16.sp
}

Button {
    id = R.id.submit
    text = "Click Me"
    onClick { showToast("Clicked!") }
}
```

## Code Generation

AeroLang compiles to C++ with JNI bindings:

**AeroLang:**
```aerolang
activity MainActivity {
    var count: Int = 0
    
    fun onCreate() {
        setContentView(R.layout.activity_main)
    }
}
```

**Generated C++:**
```cpp
#include <aerolang/Activity.h>

class MainActivity : public aero::Activity {
private:
    int32_t count = 0;
    
public:
    void onCreate() override {
        setContentView(R::layout::activity_main);
    }
};
```

## Reserved Keywords

`activity`, `class`, `fun`, `var`, `val`, `if`, `else`, `when`, `for`, `while`, `return`, `true`, `false`, `null`, `override`, `private`, `public`, `protected`

## File Extension

`.aero`
