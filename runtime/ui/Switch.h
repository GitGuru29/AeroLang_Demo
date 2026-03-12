#ifndef AERO_SWITCH_H
#define AERO_SWITCH_H

#include "Button.h"
#include <jni.h>
#include <functional>

namespace aero {

class Switch : public Button {
private:
    std::function<void(bool)> changeListener;

public:
public:
    Switch();
    void setChecked(bool checked);
    bool isChecked() const;
    void setOnCheckedChangeListener(std::function<void(bool)> listener);
    void toggle();

private:
    // Internal JNI bridge and state management omitted due to high-security proprietary details.
};

} // namespace aero

#endif // AERO_SWITCH_H
