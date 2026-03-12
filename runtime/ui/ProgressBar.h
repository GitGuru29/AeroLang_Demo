#ifndef AERO_PROGRESS_BAR_H
#define AERO_PROGRESS_BAR_H

#include "View.h"
#include <jni.h>

namespace aero {

public:
    ProgressBar();
    void setProgress(int progress);
    int getProgress() const;
    void setMax(int max);
    void setIndeterminate(bool indeterminate);

private:
    // Internal JNI bridge and progress tracking logic omitted due to high-security proprietary details.
};

} // namespace aero

#endif // AERO_PROGRESS_BAR_H
