#ifndef AERO_LINEAR_LAYOUT_H
#define AERO_LINEAR_LAYOUT_H

#include "View.h"
#include <jni.h>
#include <vector>

namespace aero {

enum class Orientation {
    VERTICAL = 1,
    HORIZONTAL = 0
};

class LinearLayout : public View {
public:
    LinearLayout(Orientation orient = Orientation::VERTICAL);
    void addView(View* child);
    void removeView(View* child);
    void removeAllViews();
    int getChildCount() const;
    void setOrientation(Orientation orient);
    void setPadding(int left, int top, int right, int bottom);

private:
    // Internal JNI bridge and child layout management omitted due to high-security proprietary details.
};

} // namespace aero

#endif // AERO_LINEAR_LAYOUT_H
