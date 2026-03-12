#ifndef AERO_MEDIA_PLAYER_H
#define AERO_MEDIA_PLAYER_H

#include <string>
#include <functional>
#include <jni.h>

namespace aero {

/**
 * MediaPlayer wrapper for playing audio and video files.
 * Provides a synchronous-looking interface to the asynchronous Android MediaPlayer API.
 */
class MediaPlayer {
public:
    MediaPlayer();
    ~MediaPlayer();

    void setDataSource(const std::string& path);
    void prepare();
    void prepareAsync();
    
    void start();
    void pause();
    void stop();
    void reset();
    void release();
    
    bool isPlaying();
    void setLooping(bool looping);
    bool isLooping();
    
    void seekTo(int msec);
    int getCurrentPosition();
    int getDuration();
    
    void setVolume(float leftVolume, float rightVolume);

    static void initJNI(JNIEnv* env);
    
    // Internal method to retrieve the underlying java object (used for VideoView/Surface binding)
    jobject getJavaObject() const;

private:
    jobject playerObj;
    JNIEnv* env;
    
    static JNIEnv* getDefaultEnv();
};

} // namespace aero

#endif // AERO_MEDIA_PLAYER_H
