#ifndef AERO_VIDEO_VIEW_H
#define AERO_VIDEO_VIEW_H

#include "View.h"
#include <jni.h>
#include <string>

namespace aero {

class VideoView : public View {
public:
    VideoView() {
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass videoViewClass = env->FindClass("android/widget/VideoView");
        jmethodID constructor = env->GetMethodID(videoViewClass, "<init>", "(Landroid/content/Context;)V");
        
        jobject context = getAndroidContext();
        viewObject = env->NewObject(videoViewClass, constructor, context);
        
        env->DeleteLocalRef(videoViewClass);
    }

    void setVideoPath(const std::string& path) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass videoViewClass = env->GetObjectClass(viewObject);
        jmethodID setVideoPathMethod = env->GetMethodID(videoViewClass, "setVideoPath", "(Ljava/lang/String;)V");
        
        jstring jPath = env->NewStringUTF(path.c_str());
        env->CallVoidMethod(viewObject, setVideoPathMethod, jPath);
        
        env->DeleteLocalRef(jPath);
        env->DeleteLocalRef(videoViewClass);
    }
    
    // Sometimes called instead of setVideoPath if parsing URI directly
    void setVideoURI(const std::string& uriStr) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass uriClass = env->FindClass("android/net/Uri");
        jmethodID parseMethod = env->GetStaticMethodID(uriClass, "parse", "(Ljava/lang/String;)Landroid/net/Uri;");
        
        jstring jUriStr = env->NewStringUTF(uriStr.c_str());
        jobject uriObj = env->CallStaticObjectMethod(uriClass, parseMethod, jUriStr);
        
        jclass videoViewClass = env->GetObjectClass(viewObject);
        jmethodID setVideoURIMethod = env->GetMethodID(videoViewClass, "setVideoURI", "(Landroid/net/Uri;)V");
        
        env->CallVoidMethod(viewObject, setVideoURIMethod, uriObj);
        
        env->DeleteLocalRef(jUriStr);
        env->DeleteLocalRef(uriObj);
        env->DeleteLocalRef(uriClass);
        env->DeleteLocalRef(videoViewClass);
    }

    void start() {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass videoViewClass = env->GetObjectClass(viewObject);
        jmethodID startMethod = env->GetMethodID(videoViewClass, "start", "()V");
        
        env->CallVoidMethod(viewObject, startMethod);
        env->DeleteLocalRef(videoViewClass);
    }

    void pause() {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass videoViewClass = env->GetObjectClass(viewObject);
        jmethodID pauseMethod = env->GetMethodID(videoViewClass, "pause", "()V");
        
        env->CallVoidMethod(viewObject, pauseMethod);
        env->DeleteLocalRef(videoViewClass);
    }

    void resume() {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass videoViewClass = env->GetObjectClass(viewObject);
        jmethodID resumeMethod = env->GetMethodID(videoViewClass, "resume", "()V");
        
        env->CallVoidMethod(viewObject, resumeMethod);
        env->DeleteLocalRef(videoViewClass);
    }
    
    void suspend() {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass videoViewClass = env->GetObjectClass(viewObject);
        jmethodID suspendMethod = env->GetMethodID(videoViewClass, "suspend", "()V");
        
        env->CallVoidMethod(viewObject, suspendMethod);
        env->DeleteLocalRef(videoViewClass);
    }
};

} // namespace aero

#endif // AERO_VIDEO_VIEW_H
