#ifndef AERO_TAB_LAYOUT_H
#define AERO_TAB_LAYOUT_H

#include "View.h"
#include "ViewPager.h"
#include <jni.h>
#include <string>
#include <functional>

namespace aero {

public:
    TabLayout();
    void addTab(const char* text);
    void setupWithViewPager(ViewPager* viewPager);
    void setOnTabSelectedListener(std::function<void(int)> listener);
    int getSelectedTabPosition() const;

private:
    // Internal JNI bridge and tab management omitted due to high-security proprietary details.
};

} // namespace aero

#endif // AERO_TAB_LAYOUT_H
