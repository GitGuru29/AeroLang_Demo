#ifndef AERO_BOTTOM_NAV_H
#define AERO_BOTTOM_NAV_H

#include "View.h"
#include <jni.h>
#include <functional>

namespace aero {

public:
    BottomNavigationView();
    void inflateMenu(int menuResourceId);
    void setSelectedItemId(int itemId);
    int getSelectedItemId() const;
    void setOnItemSelectedListener(std::function<void(int)> listener);
    void setItemIconTintList(int colorStateList);

private:
    // Internal JNI bridge and menu management omitted due to high-security proprietary details.
};

} // namespace aero

#endif // AERO_BOTTOM_NAV_H
