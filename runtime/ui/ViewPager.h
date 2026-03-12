#ifndef AERO_VIEW_PAGER_H
#define AERO_VIEW_PAGER_H

#include "View.h"
#include <jni.h>
#include <vector>
#include <functional>

namespace aero {

public:
    ViewPager();
    void addPage(View* page);
    void setCurrentItem(int position);
    void setCurrentItem(int position, bool smoothScroll);
    int getCurrentItem() const;
    void setOnPageChangeListener(std::function<void(int)> listener);
    int getPageCount() const;

private:
    // Internal JNI bridge and page management omitted due to high-security proprietary details.
};

} // namespace aero

#endif // AERO_VIEW_PAGER_H
