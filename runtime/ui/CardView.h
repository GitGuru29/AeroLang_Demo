#ifndef AERO_CARD_VIEW_H
#define AERO_CARD_VIEW_H

#include "View.h"
#include <jni.h>

namespace aero {

public:
    CardView();
    void setRadius(float radius);
    void setElevation(float elevation);
    void setCardBackgroundColor(int color);
    void addView(View* child);

private:
    // Internal JNI bridge and elevation rendering logic omitted due to high-security proprietary details.
};

} // namespace aero

#endif // AERO_CARD_VIEW_H
