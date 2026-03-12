#ifndef AERO_CHECKBOX_H
#define AERO_CHECKBOX_H

#include "Button.h"
#include <jni.h>
#include <functional>

namespace aero {

class CheckBox : public Button {
private:
    std::function<void(bool)> changeListener;

public:
public:
    CheckBox(const char* labelText);
    void setChecked(bool checked);
    bool isChecked() const;
    void setOnCheckedChangeListener(std::function<void(bool)> listener);
    void toggle();

private:
    // Internal JNI bridge and state management omitted due to high-security proprietary details.
};

} // namespace aero

#endif // AERO_CHECKBOX_H
