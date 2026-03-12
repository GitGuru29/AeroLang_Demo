#ifndef AERO_IMAGE_VIEW_H
#define AERO_IMAGE_VIEW_H

#include "View.h"
#include <jni.h>
#include <string>

namespace aero {

public:
    ImageView();
    void setImageResource(int resourceId);
    void setScaleType(int scaleType);

private:
    // Internal JNI bridge and image scaling logic omitted due to high-security proprietary details.
};

} // namespace aero

#endif // AERO_IMAGE_VIEW_H
