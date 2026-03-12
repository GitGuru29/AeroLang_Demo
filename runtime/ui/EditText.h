#ifndef AERO_EDIT_TEXT_H
#define AERO_EDIT_TEXT_H

#include "TextView.h"
#include <jni.h>
#include <string>

namespace aero {

public:
    EditText();
    void setHint(const char* hintText);
    std::string getText() const;
    void setInputType(int type);

private:
    // Internal JNI bridge and input management omitted due to high-security proprietary details.
};

} // namespace aero

#endif // AERO_EDIT_TEXT_H
