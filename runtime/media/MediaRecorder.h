#ifndef AERO_MEDIA_RECORDER_H
#define AERO_MEDIA_RECORDER_H

#include <string>
#include <jni.h>

namespace aero {

/**
 * MediaRecorder wrapper for recording audio and video.
 */
class MediaRecorder {
public:
    MediaRecorder();
    ~MediaRecorder();

    // Audio sources (constants matching standard Android values)
    static constexpr int AUDIO_SOURCE_DEFAULT = 0;
    static constexpr int AUDIO_SOURCE_MIC = 1;
    
    // Video sources
    static constexpr int VIDEO_SOURCE_DEFAULT = 0;
    static constexpr int VIDEO_SOURCE_CAMERA = 1;

    // Output formats
    static constexpr int OUTPUT_FORMAT_DEFAULT = 0;
    static constexpr int OUTPUT_FORMAT_THREE_GPP = 1;
    static constexpr int OUTPUT_FORMAT_MPEG_4 = 2;

    // Audio encoders
    static constexpr int AUDIO_ENCODER_DEFAULT = 0;
    static constexpr int AUDIO_ENCODER_AMR_NB = 1;
    static constexpr int AUDIO_ENCODER_AAC = 3;

    // Video encoders
    static constexpr int VIDEO_ENCODER_DEFAULT = 0;
    static constexpr int VIDEO_ENCODER_H263 = 1;
    static constexpr int VIDEO_ENCODER_H264 = 2;

    void setAudioSource(int audio_source);
    void setVideoSource(int video_source);
    void setOutputFormat(int output_format);
    void setAudioEncoder(int audio_encoder);
    void setVideoEncoder(int video_encoder);
    void setOutputFile(const std::string& path);
    void setVideoSize(int width, int height);
    void setVideoFrameRate(int rate);
    void setVideoEncodingBitRate(int bitRate);
    
    void prepare();
    void start();
    void stop();
    void reset();
    void release();

    static void initJNI(JNIEnv* env);

private:
    jobject recorderObj;
    JNIEnv* env;
    
    static JNIEnv* getDefaultEnv();
};

} // namespace aero

#endif // AERO_MEDIA_RECORDER_H
