#ifndef AERO_SCROLL_VIEW_H
#define AERO_SCROLL_VIEW_H

#include "View.h"
#include <jni.h>

namespace aero {

public:
    ScrollView();
    void addView(View* child);
    void scrollTo(int x, int y);
    void fullScroll(int direction);

private:
    // Internal JNI bridge and scroll management omitted due to high-security proprietary details.
};

} // namespace aero

#endif // AERO_SCROLL_VIEW_H
