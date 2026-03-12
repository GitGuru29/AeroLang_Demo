#ifndef AERO_VIDEO_VIEW_H
#define AERO_VIDEO_VIEW_H

#include "View.h"
#include <jni.h>
#include <string>

namespace aero {

public:
    VideoView();
    void setVideoPath(const std::string& path);
    void setVideoURI(const std::string& uriStr);
    void start();
    void pause();
    void resume();
    void suspend();

private:
    // Internal JNI bridge and video surface management omitted due to high-security proprietary details.
};

} // namespace aero

#endif // AERO_VIDEO_VIEW_H
