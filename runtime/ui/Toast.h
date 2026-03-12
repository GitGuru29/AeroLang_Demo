#ifndef AERO_TOAST_H
#define AERO_TOAST_H

#include <jni.h>
#include <string>

namespace aero {

class Toast {
public:
    static void makeText(const char* message, int duration = 0);

private:
    // Internal JNI bridge and static context management omitted due to high-security proprietary details.
};

// Helper function
inline void showToast(const char* message) {
    Toast::makeText(message, 0);
}

} // namespace aero

#endif // AERO_TOAST_H
