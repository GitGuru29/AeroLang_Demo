#ifndef AERO_GESTURE_DETECTOR_H
#define AERO_GESTURE_DETECTOR_H

#include <jni.h>
#include <functional>

namespace aero {

class GestureDetector {
private:
    jobject gestureDetectorObject;
    std::function<void(float, float)> tapListener;
    std::function<void(float, float)> longPressListener;
    std::function<void(float, float, float, float)> swipeListener;
    std::function<void(float, float, float, float)> scrollListener;

public:
    GestureDetector() : gestureDetectorObject(nullptr),
                        tapListener(nullptr),
                        longPressListener(nullptr),
                        swipeListener(nullptr),
                        scrollListener(nullptr) {
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass gestureDetectorClass = env->FindClass("android/view/GestureDetector");
        jmethodID constructor = env->GetMethodID(gestureDetectorClass, "<init>", 
            "(Landroid/content/Context;Landroid/view/GestureDetector$OnGestureListener;)V");
        
        jobject context = getAndroidContext();
        // In real implementation, create listener proxy
        gestureDetectorObject = env->NewObject(gestureDetectorClass, constructor, context, nullptr);
        
        env->DeleteLocalRef(gestureDetectorClass);
    }

    void setOnTapListener(std::function<void(float, float)> listener) {
        tapListener = listener;
    }

    void setOnLongPressListener(std::function<void(float, float)> listener) {
        longPressListener = listener;
    }

    void setOnSwipeListener(std::function<void(float, float, float, float)> listener) {
        swipeListener = listener;
    }

    void setOnScrollListener(std::function<void(float, float, float, float)> listener) {
        scrollListener = listener;
    }

    // Called from JNI
    void onTap(float x, float y) {
        if (tapListener) {
            tapListener(x, y);
        }
    }

    void onLongPress(float x, float y) {
        if (longPressListener) {
            longPressListener(x, y);
        }
    }

    void onSwipe(float x1, float y1, float x2, float y2) {
        if (swipeListener) {
            swipeListener(x1, y1, x2, y2);
        }
    }

    void onScroll(float x1, float y1, float distanceX, float distanceY) {
        if (scrollListener) {
            scrollListener(x1, y1, distanceX, distanceY);
        }
    }

    ~GestureDetector() {
        if (gestureDetectorObject) {
            JNIEnv* env = getJNIEnv();
            if (env) {
                env->DeleteGlobalRef(gestureDetectorObject);
            }
        }
    }

private:
    static JNIEnv* getJNIEnv() {
        static JNIEnv* env = nullptr;
        return env;
    }

    static jobject getAndroidContext() {
        static jobject context = nullptr;
        return context;
    }
};

} // namespace aero

#endif // AERO_GESTURE_DETECTOR_H
