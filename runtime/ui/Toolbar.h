#ifndef AERO_TOOLBAR_H
#define AERO_TOOLBAR_H

#include "View.h"
#include <jni.h>
#include <string>
#include <functional>

namespace aero {

public:
    Toolbar();
    void setTitle(const char* title);
    void setSubtitle(const char* subtitle);
    void setNavigationIcon(int resourceId);
    void setNavigationOnClickListener(std::function<void()> listener);
    void inflateMenu(int menuResourceId);
    void setOnMenuItemClickListener(std::function<void(int)> listener);

private:
    // Internal JNI bridge and menu management omitted due to high-security proprietary details.
};

} // namespace aero

#endif // AERO_TOOLBAR_H
