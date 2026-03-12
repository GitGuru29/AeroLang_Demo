#ifndef AERO_FAB_H
#define AERO_FAB_H

#include "View.h"
#include <jni.h>
#include <functional>

namespace aero {

public:
    FloatingActionButton();
    void setIcon(int resourceId);
    void setOnClickListener(std::function<void()> listener);
    void setSize(int size);
    void show();
    void hide();

private:
    // Internal JNI bridge and FAB state management omitted due to high-security proprietary details.
};

} // namespace aero

#endif // AERO_FAB_H
