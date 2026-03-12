#ifndef AERO_DRAWER_LAYOUT_H
#define AERO_DRAWER_LAYOUT_H

#include "View.h"
#include <jni.h>
#include <functional>

namespace aero {

class NavigationView : public View {
public:
    NavigationView();
    void inflateMenu(int menuResourceId);
    void setNavigationItemSelectedListener(std::function<void(int)> listener);

private:
    // Internal JNI bridge and navigation menu logic omitted due to high-security proprietary details.
};

class DrawerLayout : public View {
public:
    DrawerLayout();
    void addView(View* child);
    void openDrawer(int gravity);
    void closeDrawer(int gravity);
    bool isDrawerOpen(int gravity) const;
    void setDrawerLockMode(int lockMode);

private:
    // Internal JNI bridge and drawer state management omitted due to high-security proprietary details.
};

} // namespace aero

#endif // AERO_DRAWER_LAYOUT_H
