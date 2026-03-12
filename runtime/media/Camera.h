#ifndef AERO_CAMERA_H
#define AERO_CAMERA_H

#include <string>
#include <jni.h>

namespace aero {

/**
 * Camera API for capturing pictures and video.
 * For MVP, we use the standard Android MediaStore Intents to capture images.
 * This handles the heavy lifting of permissions and UI, saving the result to a specified path.
 */
class Camera {
public:
    Camera();
    ~Camera();
    
    // Set the path where the captured image should be saved
    void setOutputPath(const std::string& path);
    std::string getOutputPath() const;
    
    // Launch the camera app to take a picture
    void takePicture();
    
    // Launch the camera app to capture a video
    void captureVideo();

    // Init JNI (called by Activity)
    static void initJNI(JNIEnv* env);

private:
    std::string outputPath;
    JNIEnv* env;
    
    static JNIEnv* getDefaultEnv();
};

} // namespace aero

#endif // AERO_CAMERA_H
