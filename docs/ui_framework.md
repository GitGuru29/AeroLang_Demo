# UI Framework

AeroLang UI components for building Android interfaces with native performance.

## Available Components

### Button
Interactive button with click events.

```aerolang
var btn = Button("Click Me")
btn.setOnClickListener(() => {
    showToast("Button clicked!")
})
layout.addView(btn)
```

### TextView
Display text with styling.

```aerolang
var text = TextView("Hello World")
text.setTextSize(24.0)
text.setTextColor(0xFF000000)  // Black
layout.addView(text)
```

### EditText
User text input field.

```aerolang
var input = EditText()
input.setHint("Enter name...")
val userInput = input.getText()
```

### LinearLayout
Organize views vertically or horizontally.

```aerolang
var layout = LinearLayout(Orientation::VERTICAL)
layout.setPadding(20, 20, 20, 20)
layout.addView(textView)
layout.addView(button)
```

## Complete Example: Counter App

```aerolang
activity CounterApp {
    var counterText: TextView
    var incrementBtn: Button
    var count: Int
    
    fun onCreate() {
        count = 0
        
        var layout = LinearLayout(Orientation::VERTICAL)
        
        counterText = TextView("Count: 0")
        counterText.setTextSize(32.0)
        layout.addView(counterText)
        
        incrementBtn = Button("Increment")
        incrementBtn.setOnClickListener(() => {
            count = count + 1
            counterText.setText("Count: " + count)
        })
        layout.addView(incrementBtn)
        
        setContentView(layout)
    }
}
```

## Event Handling

All interactive components support lambda-based event handlers:

```aerolang
button.setOnClickListener(() => {
    // Your code here
})
```

## Layout Management

### Vertical Layout
```aerolang
var layout = LinearLayout(Orientation::VERTICAL)
```

### Horizontal Layout
```aerolang
var layout = LinearLayout(Orientation::HORIZONTAL)
```

### Adding/Removing Views
```aerolang
layout.addView(view)
layout.removeView(view)
layout.removeAllViews()
```

## Styling

### Text Size
```aerolang
textView.setTextSize(24.0)
```

### Colors
```aerolang
// Format: 0xAARRGGBB
textView.setTextColor(0xFFFF0000)  // Red
textView.setTextColor(0xFF00FF00)  // Green
```

### Padding
```aerolang
layout.setPadding(left, top, right, bottom)
```

## Performance

All UI components map directly to Android native views:
- ✅ **No overhead**: Direct JNI calls
- ✅ **Native rendering**: Same as Java/Kotlin
- ✅ **60 FPS**: Hardware accelerated

## Coming Soon

- RecyclerView (lists)
- ImageView
- ScrollView
- ConstraintLayout
- Material Design components
