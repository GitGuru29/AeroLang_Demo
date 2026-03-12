#ifndef AERO_SNACKBAR_H
#define AERO_SNACKBAR_H

#include <jni.h>
#include <string>
#include <functional>

namespace aero {

public:
    static void make(const char* message, const char* actionText, std::function<void()> actionCallback, int duration = 0);
    static void makeShort(const char* message);
    static void makeLong(const char* message);

private:
    // Internal JNI bridge and static root view management omitted due to high-security proprietary details.
};

} // namespace aero

#endif // AERO_SNACKBAR_H
