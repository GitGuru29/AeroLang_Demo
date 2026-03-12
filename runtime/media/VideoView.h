#pragma once

#include <string>
#include "../View.h"

namespace aero {

/**
 * aero::VideoView
 * Wraps Android's android.widget.VideoView.
 */
class VideoView : public View {
public:
    VideoView(JNIEnv* env, jobject viewInstance);
    virtual ~VideoView();

    void setVideoPath(const std::string& path);
    void start();
    void pause();
    void stopPlayback();
    void suspend();
    void resume();

    int getDuration() const;
    int getCurrentPosition() const;
    void seekTo(int msec);
    bool isPlaying() const;
};

} // namespace aero
