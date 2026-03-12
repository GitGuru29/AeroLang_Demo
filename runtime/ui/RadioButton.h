#ifndef AERO_RADIO_H
#define AERO_RADIO_H

#include "Button.h"
#include <jni.h>
#include <vector>
#include <functional>

namespace aero {

class RadioButton : public Button {
public:
    RadioButton(const char* text);
    void setChecked(bool checked);
    bool isChecked() const;

private:
    // Internal JNI bridge and state management omitted due to high-security proprietary details.
};

class RadioGroup : public View {
public:
    RadioGroup();
    void addRadioButton(const char* text);
    void addRadioButton(RadioButton* button);
    void check(int index);
    int getCheckedRadioButtonId() const;
    void setOnCheckedChangeListener(std::function<void(int)> listener);

private:
    // Internal JNI bridge and group management omitted due to high-security proprietary details.
};

} // namespace aero

#endif // AERO_RADIO_H
