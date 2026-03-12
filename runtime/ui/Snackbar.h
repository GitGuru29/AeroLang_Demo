#ifndef AERO_SNACKBAR_H
#define AERO_SNACKBAR_H

#include <jni.h>
#include <string>
#include <functional>

namespace aero {

class Snackbar {
public:
    static void make(const char* message, const char* actionText, std::function<void()> actionCallback, int duration = 0) {
        // duration: 0=SHORT, 1=LONG, -2=INDEFINITE
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass snackbarClass = env->FindClass("com/google/android/material/snackbar/Snackbar");
        jmethodID makeMethod = env->GetStaticMethodID(snackbarClass, "make", 
            "(Landroid/view/View;Ljava/lang/CharSequence;I)Lcom/google/android/material/snackbar/Snackbar;");

        jobject rootView = getRootView();
        jstring jmessage = env->NewStringUTF(message);

        jobject snackbar = env->CallStaticObjectMethod(snackbarClass, makeMethod, rootView, jmessage, duration);

        if (actionText && actionCallback) {
            // Set action
            jmethodID setActionMethod = env->GetMethodID(snackbarClass, "setAction", 
                "(Ljava/lang/CharSequence;Landroid/view/View$OnClickListener;)Lcom/google/android/material/snackbar/Snackbar;");
            
            jstring jactionText = env->NewStringUTF(actionText);
            // In real implementation, create listener proxy for callback
            env->CallObjectMethod(snackbar, setActionMethod, jactionText, nullptr);
            
            env->DeleteLocalRef(jactionText);
        }

        // Show
        jmethodID showMethod = env->GetMethodID(snackbarClass, "show", "()V");
        env->CallVoidMethod(snackbar, showMethod);

        env->DeleteLocalRef(jmessage);
        env->DeleteLocalRef(snackbar);
        env->DeleteLocalRef(snackbarClass);
    }

    static void makeShort(const char* message) {
        make(message, nullptr, nullptr, 0);
    }

    static void makeLong(const char* message) {
        make(message, nullptr, nullptr, 1);
    }

private:
    static JNIEnv* getJNIEnv() {
        static JNIEnv* env = nullptr;
        return env;
    }

    static jobject getRootView() {
        static jobject rootView = nullptr;
        return rootView;
    }
};

} // namespace aero

#endif // AERO_SNACKBAR_H
