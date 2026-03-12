# Custom Gesture System Documentation

## Overview

AeroLang provides a **fully customizable gesture system** allowing you to define your own swipe patterns, thresholds, and behaviors.

## CustomGestureListener

### Basic Usage

```aerolang
var gestureListener = CustomGestureListener()

gestureListener.onSwipe((direction) => {
    showToast("Swiped: " + direction)
})
```

## Configuration

### Swipe Sensitivity

```aerolang
gestureListener.setMinSwipeDistance(150.0)      // Minimum pixels
              .setMaxSwipeDuration(600.0)       // Max milliseconds
              .setVelocityThreshold(200.0)      // Min pixels/second
              .setAngleThreshold(30.0)          // Angle tolerance (degrees)
```

## Supported Gestures

### 1. Swipe Detection

```aerolang
// Basic swipe
gestureListener.onSwipe((direction) => {
    // direction: UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT
})

// Swipe with velocity
gestureListener.onSwipeWithVelocity((direction, velocity) => {
    if (velocity > 500.0) {
        showToast("FAST swipe!")
    }
})
```

### 2. Tap Gestures

```aerolang
// Single tap
gestureListener.onTap((x, y) => {
    showToast("Tapped at " + x + ", " + y)
})

// Double tap
gestureListener.onDoubleTap((x, y) => {
    showToast("Double tapped!")
})
```

### 3. Long Press

```aerolang
gestureListener.onLongPress((x, y, duration) => {
    showToast("Held for " + duration + "ms")
})
```

### 4. Multi-Touch Gestures

```aerolang
// Pinch to zoom
gestureListener.onPinch((scale, distance) => {
    zoom = zoom * scale
})

// Rotate
gestureListener.onRotate((angle) => {
    rotation = rotation + angle
})
```

### 5. Custom Swipe Patterns

```aerolang
gestureListener.onCustomSwipe((x1, y1, x2, y2) => {
    val dx = x2 - x1
    val dy = y2 - y1
    
    // Define your own pattern
    if (dx > 100.0 && dy < -100.0) {
        showToast("Diagonal up-right!")
    }
})
```

## Swipe Directions

```aerolang
SwipeDirection::UP
SwipeDirection::DOWN
SwipeDirection::LEFT
SwipeDirection::RIGHT
SwipeDirection::UP_LEFT
SwipeDirection::UP_RIGHT
SwipeDirection::DOWN_LEFT
SwipeDirection::DOWN_RIGHT
```

## Real-World Examples

### Tinder-Style Swipe

```aerolang
gestureListener.setMinSwipeDistance(150.0)
              .onSwipe((direction) => {
                  if (direction == SwipeDirection::RIGHT) {
                      like()
                  } else if (direction == SwipeDirection::LEFT) {
                      dislike()
                  }
              })
```

### Instagram-Style Double Tap to Like

```aerolang
gestureListener.onDoubleTap((x, y) => {
    likePost()
    showHeart(x, y)
})
```

### Photo Editor Pinch Zoom

```aerolang
gestureListener.onPinch((scale, distance) => {
    imageZoom = imageZoom * scale
    applyZoom(imageZoom)
})
```

### Gaming Swipe Controls

```aerolang
gestureListener.setVelocityThreshold(300.0)
              .onSwipeWithVelocity((direction, velocity) => {
                  if (velocity > 800.0) {
                      // Fast swipe = power move
                      powerAttack(direction)
                  } else {
                      // Normal swipe = regular move
                      normalAttack(direction)
                  }
              })
```

## Advanced: Builder Pattern

Chain multiple configurations:

```aerolang
gestureListener
    .setMinSwipeDistance(100.0)
    .setMaxSwipeDuration(500.0)
    .setVelocityThreshold(150.0)
    .onSwipe((dir) => { handleSwipe(dir) })
    .onDoubleTap((x, y) => { handleDoubleTap(x, y) })
    .onLongPress((x, y, dur) => { handleLongPress(x, y, dur) })
```

## Performance Notes

- All gesture detection runs in native C++
- Zero overhead for unused gestures
- Velocity calculated in pixels/second
- Timestamps use system time (microseconds)

## Complete Example: Card Swipe App

```aerolang
activity CardSwipeApp {
    var gesture: CustomGestureListener
    
    fun onCreate() {
        gesture = CustomGestureListener()
        
        gesture.setMinSwipeDistance(120.0)
               .setVelocityThreshold(200.0)
               .onSwipeWithVelocity((dir, vel) => {
                   if (dir == SwipeDirection::RIGHT) {
                       like(vel > 500.0)  // Fast = super like
                   } else if (dir == SwipeDirection::LEFT) {
                       dislike()
                   }
               })
               .onDoubleTap((x, y) => {
                   superLike()
               })
    }
}
```

## Tips

1. **Adjust sensitivity**: Use `setMinSwipeDistance()` for larger/smaller swipes
2. **Velocity-based actions**: Check velocity for "fast" vs "slow" gestures
3. **Diagonal swipes**: Use `onCustomSwipe()` for complex patterns
4. **Multi-touch**: Use `onPinch()` and `onRotate()` for zoom/rotate
5. **Combine gestures**: Mix tap, swipe, and long press for rich interactions

---

**You now have COMPLETE gesture control!** 🎮
