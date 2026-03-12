#ifndef AERO_FAB_H
#define AERO_FAB_H

#include "View.h"
#include <jni.h>
#include <functional>

namespace aero {

class FloatingActionButton : public View {
private:
    std::function<void()> clickListener;

public:
    FloatingActionButton() : clickListener(nullptr) {
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass fabClass = env->FindClass("com/google/android/material/floatingactionbutton/FloatingActionButton");
        jmethodID constructor = env->GetMethodID(fabClass, "<init>", "(Landroid/content/Context;)V");
        
        jobject context = getAndroidContext();
        viewObject = env->NewObject(fabClass, constructor, context);
        
        env->DeleteLocalRef(fabClass);
    }

    void setIcon(int resourceId) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass fabClass = env->GetObjectClass(viewObject);
        jmethodID setImageResourceMethod = env->GetMethodID(fabClass, "setImageResource", "(I)V");
        
        env->CallVoidMethod(viewObject, setImageResourceMethod, resourceId);
        env->DeleteLocalRef(fabClass);
    }

    void setOnClickListener(std::function<void()> listener) {
        clickListener = listener;
        // Set up JNI callback
    }

    void setSize(int size) {
        // SIZE_NORMAL=0, SIZE_MINI=1, SIZE_AUTO=-1
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass fabClass = env->GetObjectClass(viewObject);
        jmethodID setSizeMethod = env->GetMethodID(fabClass, "setSize", "(I)V");
        
        env->CallVoidMethod(viewObject, setSizeMethod, size);
        env->DeleteLocalRef(fabClass);
    }

    void show() {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass fabClass = env->GetObjectClass(viewObject);
        jmethodID showMethod = env->GetMethodID(fabClass, "show", "()V");
        
        env->CallVoidMethod(viewObject, showMethod);
        env->DeleteLocalRef(fabClass);
    }

    void hide() {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass fabClass = env->GetObjectClass(viewObject);
        jmethodID hideMethod = env->GetMethodID(fabClass, "hide", "()V");
        
        env->CallVoidMethod(viewObject, hideMethod);
        env->DeleteLocalRef(fabClass);
    }
};

} // namespace aero

#endif // AERO_FAB_H
