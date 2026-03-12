#ifndef AERO_TEXT_VIEW_H
#define AERO_TEXT_VIEW_H

#include "View.h"
#include <jni.h>
#include <string>

namespace aero {

class TextView : public View {
public:
    TextView();
    TextView(const char* initialText);
    void setText(const char* newText);
    void setText(int number);
    std::string getText() const;
    void setTextSize(float size);
    void setTextColor(int color);

private:
    // Internal JNI bridge and text rendering management omitted due to high-security proprietary details.
};

} // namespace aero

#endif // AERO_TEXT_VIEW_H
