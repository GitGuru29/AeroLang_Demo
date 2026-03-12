# Navigation System Guide

Complete guide to using AeroLang's navigation system for building multi-screen Android apps.

## Table of Contents
1. [Introduction](#introduction)
2. [Intent Basics](#intent-basics)
3. [Activity Navigation](#activity-navigation)
4. [Data Passing](#data-passing)
5. [Activity Results](#activity-results)
6. [Implicit Intents](#implicit-intents)
7. [Best Practices](#best-practices)

---

## Introduction

AeroLang's navigation system enables you to build multi-screen applications using the Intent and Activity APIs. This system provides a familiar, Android-native approach to screen transitions and data passing.

**Key Features:**
- ✅ Explicit intents for navigating to specific activities
- ✅ Implicit intents for action-based navigation
- ✅ Type-safe data passing with `putExtra()` / `getExtra()`
- ✅ Activity results with `startActivityForResult()`
- ✅ Complete lifecycle management
- ✅ Back stack navigation with `finish()`

---

## Intent Basics

### What is an Intent?

An Intent is a messaging object used to request an action from another app component. In AeroLang, Intents are primarily used to start activities.

### Creating an Explicit Intent

Navigate to a specific activity:

```aerolang
val intent = Intent(this, ProfileActivity::class)
startActivity(intent)
```

### Creating an Implicit Intent

Request an action (share, view, etc.):

```aerolang
val intent = Intent(Intent.ACTION_VIEW)
intent.setData("https://aerolang.dev")
startActivity(intent)
```

---

## Activity Navigation

### Basic Navigation

**Navigate from MainActivity to ProfileActivity:**

```aerolang
activity MainActivity {
    fun onCreate() {
        showToast("Main Activity")
    }
    
    fun openProfile() {
        val intent = Intent(this, ProfileActivity::class)
        startActivity(intent)
    }
}

activity ProfileActivity {
    fun onCreate() {
        showToast("Profile Activity")
    }
    
    fun goBack() {
        finish()  // Return to previous activity
    }
}
```

### Navigation with Flags

Control the activity stack:

```aerolang
fun navigateToMain() {
    val intent = Intent(this, MainActivity::class)
    
    // Clear all activities above MainActivity
    intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP)
    
    // Don't keep this activity in history
    intent.addFlags(Intent.FLAG_ACTIVITY_NO_HISTORY)
    
    startActivity(intent)
}
```

**Common Flags:**
- `FLAG_ACTIVITY_NEW_TASK` - Start activity in a new task
- `FLAG_ACTIVITY_CLEAR_TOP` - Clear all activities above target
- `FLAG_ACTIVITY_SINGLE_TOP` - Don't create duplicate if already on top
- `FLAG_ACTIVITY_CLEAR_TASK` - Clear entire task
- `FLAG_ACTIVITY_NO_HISTORY` - Don't keep in back stack

---

## Data Passing

### Sending Data

Pass data using `putExtra()`:

```aerolang
activity MainActivity {
    fun openProfile() {
        val intent = Intent(this, ProfileActivity::class)
        
        // Pass different data types
        intent.putExtra("userId", 42)
        intent.putExtra("userName", "Alice")
        intent.putExtra("score", 9500)
        intent.putExtra("isPremium", true)
        intent.putExtra("rating", 4.8)
        
        startActivity(intent)
    }
}
```

### Receiving Data

Retrieve data using `getExtra()`:

```aerolang
activity ProfileActivity {
    var userId: Int = 0
    var userName: String = ""
    var isPremium: Boolean = false
    
    fun onCreate() {
        val intent = getIntent()
        
        // Get data with default values
        userId = intent.getIntExtra("userId", 0)
        userName = intent.getStringExtra("userName", "Unknown")
        isPremium = intent.getBooleanExtra("isPremium", false)
        
        showToast("User: " + userName + " (ID: " + userId + ")")
    }
}
```

### Supported Data Types

| Method | Type | Example |
|--------|------|---------|
| `putExtra(key, value)` | `Int` (int32) | `intent.putExtra("count", 123)` |
| `putExtra(key, value)` | `Long` (int64) | `intent.putExtra("id", 123456789)` |
| `putExtra(key, value)` | `Float` | `intent.putExtra("price", 19.99)` |
| `putExtra(key, value)` | `Double` | `intent.putExtra("rating", 4.85)` |
| `putExtra(key, value)` | `Boolean` | `intent.putExtra("isActive", true)` |
| `putExtra(key, value)` | `String` | `intent.putExtra("name", "Alice")` |

### Checking for Data

```aerolang
if (intent.hasExtra("userId")) {
    userId = intent.getIntExtra("userId", 0)
} else {
    showToast("No user ID provided")
}
```

---

## Activity Results

Use `startActivityForResult()` when you need data back from the launched activity.

### Requesting a Result

```aerolang
activity MainActivity {
    fun openSettings() {
        val intent = Intent(this, SettingsActivity::class)
        startActivityForResult(intent, 100)  // Request code 100
    }
    
    fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent) {
        if (requestCode == 100) {
            if (resultCode == RESULT_OK) {
                val theme = data.getStringExtra("selectedTheme", "light")
                showToast("Theme changed to: " + theme)
            } else if (resultCode == RESULT_CANCELED) {
                showToast("Settings canceled")
            }
        }
    }
}
```

### Returning a Result

```aerolang
activity SettingsActivity {
    fun saveAndReturn(theme: String) {
        val resultIntent = Intent(this, MainActivity::class)
        resultIntent.putExtra("selectedTheme", theme)
        
        setResult(RESULT_OK, resultIntent)
        finish()
    }
    
    fun cancel() {
        setResult(RESULT_CANCELED)
        finish()
    }
}
```

**Result Codes:**
- `RESULT_OK` = -1 (success)
- `RESULT_CANCELED` = 0 (canceled)
- `RESULT_FIRST_USER` = 1 (custom result codes start here)

---

## Implicit Intents

Use implicit intents to request actions from other apps.

### Open a Website

```aerolang
fun openWebsite() {
    val intent = Intent(Intent.ACTION_VIEW)
    intent.setData("https://aerolang.dev")
    startActivity(intent)
}
```

### Share Content

```aerolang
fun shareText() {
    val intent = Intent(Intent.ACTION_SEND)
    intent.setType("text/plain")
    intent.putExtra("android.intent.extra.TEXT", "Check out AeroLang!")
    startActivity(intent)
}
```

### Make a Phone Call

```aerolang
fun dialNumber() {
    val intent = Intent(Intent.ACTION_DIAL)
    intent.setData("tel:+1234567890")
    startActivity(intent)
}
```

### Send Email

```aerolang
fun sendEmail() {
    val intent = Intent(Intent.ACTION_SENDTO)
    intent.setData("mailto:hello@aerolang.dev")
    intent.putExtra("android.intent.extra.SUBJECT", "Hello from AeroLang")
    startActivity(intent)
}
```

**Common Actions:**
- `Intent.ACTION_VIEW` - Display data
- `Intent.ACTION_SEND` - Share content
- `Intent.ACTION_SENDTO` - Send to specific target
- `Intent.ACTION_DIAL` - Dial phone number
- `Intent.ACTION_CALL` - Make phone call (requires permission)
- `Intent.ACTION_EDIT` - Edit data
- `Intent.ACTION_PICK` - Pick item from data

---

## Best Practices

### 1. Always Provide Default Values

```aerolang
// ✅ GOOD - Provides fallback
val userId = intent.getIntExtra("userId", -1)

// ❌ BAD - No fallback if key doesn't exist
val userId = intent.getIntExtra("userId", 0)  // 0 might be a valid ID!
```

### 2. Check Intent Data Exists

```aerolang
if (intent.hasExtra("userId")) {
    // Safe to use
    val userId = intent.getIntExtra("userId", 0)
}
```

### 3. Use Descriptive Request Codes

```aerolang
// ✅ GOOD - Clear intent
val REQUEST_SETTINGS = 100
val REQUEST_PROFILE = 101
val REQUEST_CAMERA = 102

startActivityForResult(intent, REQUEST_SETTINGS)

// ❌ BAD - Magic numbers
startActivityForResult(intent, 1)
```

### 4. Handle Activity Results Properly

```aerolang
fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent) {
    // Always check request code
    if (requestCode == REQUEST_SETTINGS) {
        // Always check result code
        if (resultCode == RESULT_OK) {
            // Always check data isn't null
            if (data != null && data.hasExtra("theme")) {
                val theme = data.getStringExtra("theme", "light")
                applyTheme(theme)
            }
        }
    }
}
```

### 5. Clean Up Navigation Stack

Prevent activity pile-up:

```aerolang
fun returnToMain() {
    val intent = Intent(this, MainActivity::class)
    intent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP)
    intent.addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP)
    startActivity(intent)
    finish()
}
```

### 6. Document Your Intent Contracts

Add comments for clarity:

```aerolang
activity ProfileActivity {
    // Expected extras:
    // - "userId" (Int): Required user ID
    // - "userName" (String): Optional user name
    // - "isPremium" (Boolean): Optional premium status
    
    fun onCreate() {
        val intent = getIntent()
        // ...
    }
}
```

---

## Complete Example

See [`examples/MultiScreenApp.aero`](../examples/MultiScreenApp.aero) for a full working example with:
- Multiple activities
- Data passing between screens
- Activity results
- Implicit intents
- Navigation stack management

---

## What's Next?

- **Fragments** - Modular UI components (Coming in Phase 2)
- **Navigation Component** - Type-safe navigation (Coming in Phase 3)
- **Deep Linking** - URL-based navigation (Coming in Phase 3)
- **Back Stack Management** - Advanced navigation patterns (Coming in Phase 4)

---

**Need Help?** Check out the [examples](../examples/) or read the [official documentation](../docs/).
