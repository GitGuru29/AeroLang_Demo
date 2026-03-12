#ifndef AERO_SEEK_BAR_H
#define AERO_SEEK_BAR_H

#include "ProgressBar.h"
#include <jni.h>
#include <functional>

namespace aero {

public:
    SeekBar();
    void setOnSeekBarChangeListener(std::function<void(int)> listener);

private:
    // Internal JNI bridge and seek logic omitted due to high-security proprietary details.
};

} // namespace aero

#endif // AERO_SEEK_BAR_H
