#ifndef AERO_BUTTON_H
#define AERO_BUTTON_H

#include "View.h"
#include <jni.h>
#include <string>
#include <functional>

namespace aero {

class Button : public View {
public:
    Button(const char* buttonText);
    void setText(const char* newText);
    std::string getText() const;
    void setOnClickListener(std::function<void()> listener);
    void performClick();
    static void onClickCallback(jlong nativePtr);

private:
    // Internal JNI bridge and click handling logic omitted due to high-security proprietary details.
};

} // namespace aero

#endif // AERO_BUTTON_H
