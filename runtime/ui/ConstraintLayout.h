#ifndef AERO_CONSTRAINT_LAYOUT_H
#define AERO_CONSTRAINT_LAYOUT_H

#include "View.h"
#include <jni.h>
#include <vector>

namespace aero {

enum class ConstraintSide {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
    START,
    END
};

public:
    ConstraintLayout();
    void addView(View* child);
    void constrainTo(View* view, View* target, ConstraintSide viewSide, ConstraintSide targetSide, int margin = 0);

private:
    // Layout children and complex constraint logic omitted due to high-security proprietary details.
};

} // namespace aero

#endif // AERO_CONSTRAINT_LAYOUT_H
