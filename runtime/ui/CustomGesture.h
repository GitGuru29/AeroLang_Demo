#ifndef AERO_CUSTOM_GESTURE_H
#define AERO_CUSTOM_GESTURE_H

#include <functional>
#include <vector>
#include <cmath>

namespace aero {

enum class SwipeDirection {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    UP_LEFT,
    UP_RIGHT,
    DOWN_LEFT,
    DOWN_RIGHT,
    NONE
};

enum class GestureType {
    TAP,
    DOUBLE_TAP,
    LONG_PRESS,
    SWIPE,
    PINCH,
    ROTATE
};

struct TouchPoint {
    float x;
    float y;
    long timestamp;
};

struct SwipeConfig {
    float minDistance;        // Minimum swipe distance
    float maxDuration;        // Maximum time for swipe
    float angleThreshold;     // Angle tolerance (degrees)
    float velocityThreshold;  // Minimum velocity
    
    SwipeConfig() 
        : minDistance(100.0f), 
          maxDuration(500.0f), 
          angleThreshold(45.0f), 
          velocityThreshold(100.0f) {}
};

class CustomGestureListener {
private:
    std::vector<TouchPoint> touchHistory;
    SwipeConfig config;
    
    // Callbacks for different gestures
    std::function<void(SwipeDirection)> onSwipeCallback;
    std::function<void(SwipeDirection, float)> onSwipeWithVelocityCallback;
    std::function<void(float, float)> onTapCallback;
    std::function<void(float, float)> onDoubleTapCallback;
    std::function<void(float, float, float)> onLongPressCallback;
    std::function<void(float, float)> onPinchCallback;
    std::function<void(float)> onRotateCallback;
    std::function<void(float, float, float, float)> onCustomSwipeCallback;

public:
    CustomGestureListener() : config() {}

    // Builder methods for configuration
    CustomGestureListener& setMinSwipeDistance(float distance) {
        config.minDistance = distance;
        return *this;
    }

    CustomGestureListener& setMaxSwipeDuration(float duration) {
        config.maxDuration = duration;
        return *this;
    }

    CustomGestureListener& setAngleThreshold(float angle) {
        config.angleThreshold = angle;
        return *this;
    }

    CustomGestureListener& setVelocityThreshold(float velocity) {
        config.velocityThreshold = velocity;
        return *this;
    }

    // Callback setters
    CustomGestureListener& onSwipe(std::function<void(SwipeDirection)> callback) {
        onSwipeCallback = callback;
        return *this;
    }

    CustomGestureListener& onSwipeWithVelocity(std::function<void(SwipeDirection, float)> callback) {
        onSwipeWithVelocityCallback = callback;
        return *this;
    }

    CustomGestureListener& onTap(std::function<void(float, float)> callback) {
        onTapCallback = callback;
        return *this;
    }

    CustomGestureListener& onDoubleTap(std::function<void(float, float)> callback) {
        onDoubleTapCallback = callback;
        return *this;
    }

    CustomGestureListener& onLongPress(std::function<void(float, float, float)> callback) {
        onLongPressCallback = callback;
        return *this;
    }

    CustomGestureListener& onPinch(std::function<void(float, float)> callback) {
        onPinchCallback = callback;
        return *this;
    }

    CustomGestureListener& onRotate(std::function<void(float)> callback) {
        onRotateCallback = callback;
        return *this;
    }

    // Custom swipe with full control
    CustomGestureListener& onCustomSwipe(std::function<void(float, float, float, float)> callback) {
        onCustomSwipeCallback = callback;
        return *this;
    }

    // Touch event handling
    void onTouchDown(float x, float y, long timestamp) {
        TouchPoint point = {x, y, timestamp};
        touchHistory.push_back(point);
    }

    void onTouchMove(float x, float y, long timestamp) {
        TouchPoint point = {x, y, timestamp};
        touchHistory.push_back(point);
    }

    void onTouchUp(float x, float y, long timestamp) {
        TouchPoint point = {x, y, timestamp};
        touchHistory.push_back(point);
        
        // Analyze gesture
        analyzeGesture();
        
        // Clear history
        touchHistory.clear();
    }

private:
    void analyzeGesture() {
        if (touchHistory.size() < 2) {
            // Single tap
            if (onTapCallback && touchHistory.size() == 1) {
                onTapCallback(touchHistory[0].x, touchHistory[0].y);
            }
            return;
        }

        TouchPoint start = touchHistory.front();
        TouchPoint end = touchHistory.back();
        
        float dx = end.x - start.x;
        float dy = end.y - start.y;
        float distance = std::sqrt(dx * dx + dy * dy);
        long duration = end.timestamp - start.timestamp;
        
        // Check if it's a swipe
        if (distance >= config.minDistance && duration <= config.maxDuration) {
            float velocity = distance / (duration / 1000.0f); // pixels per second
            
            if (velocity >= config.velocityThreshold) {
                SwipeDirection direction = calculateDirection(dx, dy);
                
                // Trigger callbacks
                if (onSwipeCallback) {
                    onSwipeCallback(direction);
                }
                
                if (onSwipeWithVelocityCallback) {
                    onSwipeWithVelocityCallback(direction, velocity);
                }
                
                if (onCustomSwipeCallback) {
                    onCustomSwipeCallback(start.x, start.y, end.x, end.y);
                }
            }
        }
    }

    SwipeDirection calculateDirection(float dx, float dy) {
        float angle = std::atan2(dy, dx) * 180.0f / M_PI;
        float absDx = std::abs(dx);
        float absDy = std::abs(dy);
        
        // Check for diagonal swipes
        if (absDx > config.minDistance && absDy > config.minDistance) {
            if (dx > 0 && dy < 0) return SwipeDirection::UP_RIGHT;
            if (dx > 0 && dy > 0) return SwipeDirection::DOWN_RIGHT;
            if (dx < 0 && dy < 0) return SwipeDirection::UP_LEFT;
            if (dx < 0 && dy > 0) return SwipeDirection::DOWN_LEFT;
        }
        
        // Cardinal directions
        if (absDx > absDy) {
            return dx > 0 ? SwipeDirection::RIGHT : SwipeDirection::LEFT;
        } else {
            return dy > 0 ? SwipeDirection::DOWN : SwipeDirection::UP;
        }
    }
};

// Helper to convert direction to string
inline const char* swipeDirectionToString(SwipeDirection dir) {
    switch (dir) {
        case SwipeDirection::UP: return "UP";
        case SwipeDirection::DOWN: return "DOWN";
        case SwipeDirection::LEFT: return "LEFT";
        case SwipeDirection::RIGHT: return "RIGHT";
        case SwipeDirection::UP_LEFT: return "UP_LEFT";
        case SwipeDirection::UP_RIGHT: return "UP_RIGHT";
        case SwipeDirection::DOWN_LEFT: return "DOWN_LEFT";
        case SwipeDirection::DOWN_RIGHT: return "DOWN_RIGHT";
        default: return "NONE";
    }
}

} // namespace aero

#endif // AERO_CUSTOM_GESTURE_H
