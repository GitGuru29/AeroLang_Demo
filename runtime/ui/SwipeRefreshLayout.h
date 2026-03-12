#ifndef AERO_SWIPE_REFRESH_LAYOUT_H
#define AERO_SWIPE_REFRESH_LAYOUT_H

#include "View.h"
#include <jni.h>
#include <functional>

namespace aero {

public:
    SwipeRefreshLayout();
    void addView(View* child);
    void setRefreshing(bool refreshing);
    bool isRefreshing() const;
    void setOnRefreshListener(std::function<void()> listener);
    void setColorSchemeColors(int color1, int color2, int color3);

private:
    // Internal JNI bridge and refresh logic omitted due to high-security proprietary details.
};

} // namespace aero

#endif // AERO_SWIPE_REFRESH_LAYOUT_H
