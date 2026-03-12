#ifndef AERO_TOAST_H
#define AERO_TOAST_H

#include <jni.h>
#include <string>

namespace aero {

class Toast {
public:
    static void makeText(const char* message, int duration = 0) {
        // duration: 0=SHORT, 1=LONG
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass toastClass = env->FindClass("android/widget/Toast");
        jmethodID makeTextMethod = env->GetStaticMethodID(toastClass, "makeText", 
            "(Landroid/content/Context;Ljava/lang/CharSequence;I)Landroid/widget/Toast;");

        jobject context = getAndroidContext();
        jstring jmessage = env->NewStringUTF(message);

        jobject toast = env->CallStaticObjectMethod(toastClass, makeTextMethod, context, jmessage, duration);

        jmethodID showMethod = env->GetMethodID(toastClass, "show", "()V");
        env->CallVoidMethod(toast, showMethod);

        env->DeleteLocalRef(jmessage);
        env->DeleteLocalRef(toast);
        env->DeleteLocalRef(toastClass);
    }

private:
    static JNIEnv* getJNIEnv() {
        static JNIEnv* env = nullptr;
        return env;
    }

    static jobject getAndroidContext() {
        static jobject context = nullptr;
        return context;
    }
};

// Helper function
inline void showToast(const char* message) {
    Toast::makeText(message, 0);
}

} // namespace aero

#endif // AERO_TOAST_H
