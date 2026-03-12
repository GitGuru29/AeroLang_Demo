#ifndef AERO_GESTURE_DETECTOR_H
#define AERO_GESTURE_DETECTOR_H

#include <jni.h>
#include <functional>

namespace aero {

class GestureDetector {
public:
    GestureDetector();
    ~GestureDetector();

    void setOnTapListener(std::function<void(float, float)> listener);
    void setOnLongPressListener(std::function<void(float, float)> listener);
    void setOnSwipeListener(std::function<void(float, float, float, float)> listener);
    void setOnScrollListener(std::function<void(float, float, float, float)> listener);

private:
    // Internal JNI bridge and gesture processing logic omitted due to high-security proprietary details.
};

} // namespace aero

#endif // AERO_GESTURE_DETECTOR_H
