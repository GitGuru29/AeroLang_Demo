#ifndef AERO_PROGRESS_BAR_H
#define AERO_PROGRESS_BAR_H

#include "View.h"
#include <jni.h>

namespace aero {

class ProgressBar : public View {
public:
    ProgressBar() {
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass progressBarClass = env->FindClass("android/widget/ProgressBar");
        jmethodID constructor = env->GetMethodID(progressBarClass, "<init>", "(Landroid/content/Context;)V");
        
        jobject context = getAndroidContext();
        viewObject = env->NewObject(progressBarClass, constructor, context);
        
        env->DeleteLocalRef(progressBarClass);
    }

    void setProgress(int progress) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass progressBarClass = env->GetObjectClass(viewObject);
        jmethodID setProgressMethod = env->GetMethodID(progressBarClass, "setProgress", "(I)V");
        
        env->CallVoidMethod(viewObject, setProgressMethod, progress);
        env->DeleteLocalRef(progressBarClass);
    }

    int getProgress() const {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return 0;

        jclass progressBarClass = env->GetObjectClass(viewObject);
        jmethodID getProgressMethod = env->GetMethodID(progressBarClass, "getProgress", "()I");
        
        jint progress = env->CallIntMethod(viewObject, getProgressMethod);
        env->DeleteLocalRef(progressBarClass);
        
        return static_cast<int>(progress);
    }

    void setMax(int max) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass progressBarClass = env->GetObjectClass(viewObject);
        jmethodID setMaxMethod = env->GetMethodID(progressBarClass, "setMax", "(I)V");
        
        env->CallVoidMethod(viewObject, setMaxMethod, max);
        env->DeleteLocalRef(progressBarClass);
    }

    void setIndeterminate(bool indeterminate) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass progressBarClass = env->GetObjectClass(viewObject);
        jmethodID setIndeterminateMethod = env->GetMethodID(progressBarClass, "setIndeterminate", "(Z)V");
        
        env->CallVoidMethod(viewObject, setIndeterminateMethod, indeterminate ? JNI_TRUE : JNI_FALSE);
        env->DeleteLocalRef(progressBarClass);
    }
};

} // namespace aero

#endif // AERO_PROGRESS_BAR_H
