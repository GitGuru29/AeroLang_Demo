#ifndef AERO_DIALOG_H
#define AERO_DIALOG_H

#include <jni.h>
#include <string>
#include <functional>

namespace aero {

class AlertDialog {
private:
    jobject dialogObject;
    std::function<void()> positiveCallback;
    std::function<void()> negativeCallback;

public:
    class Builder {
    private:
        jobject builderObject;
        std::function<void()> positiveCallback;
        std::function<void()> negativeCallback;

    public:
        Builder() : builderObject(nullptr), positiveCallback(nullptr), negativeCallback(nullptr) {
            JNIEnv* env = getJNIEnv();
            if (!env) return;

            jclass builderClass = env->FindClass("android/app/AlertDialog$Builder");
            jmethodID constructor = env->GetMethodID(builderClass, "<init>", "(Landroid/content/Context;)V");
            
            jobject context = getAndroidContext();
            builderObject = env->NewObject(builderClass, constructor, context);
            
            env->DeleteLocalRef(builderClass);
        }

        Builder& setTitle(const char* title) {
            JNIEnv* env = getJNIEnv();
            if (!env || !builderObject) return *this;

            jclass builderClass = env->GetObjectClass(builderObject);
            jmethodID setTitleMethod = env->GetMethodID(builderClass, "setTitle", 
                "(Ljava/lang/CharSequence;)Landroid/app/AlertDialog$Builder;");
            
            jstring jtitle = env->NewStringUTF(title);
            env->CallObjectMethod(builderObject, setTitleMethod, jtitle);
            
            env->DeleteLocalRef(jtitle);
            env->DeleteLocalRef(builderClass);
            return *this;
        }

        Builder& setMessage(const char* message) {
            JNIEnv* env = getJNIEnv();
            if (!env || !builderObject) return *this;

            jclass builderClass = env->GetObjectClass(builderObject);
            jmethodID setMessageMethod = env->GetMethodID(builderClass, "setMessage", 
                "(Ljava/lang/CharSequence;)Landroid/app/AlertDialog$Builder;");
            
            jstring jmessage = env->NewStringUTF(message);
            env->CallObjectMethod(builderObject, setMessageMethod, jmessage);
            
            env->DeleteLocalRef(jmessage);
            env->DeleteLocalRef(builderClass);
            return *this;
        }

        Builder& setPositiveButton(const char* text, std::function<void()> callback) {
            positiveCallback = callback;
            // In real implementation, would set up JNI callback
            return *this;
        }

        Builder& setNegativeButton(const char* text, std::function<void()> callback) {
            negativeCallback = callback;
            return *this;
        }

        AlertDialog* create() {
            JNIEnv* env = getJNIEnv();
            if (!env || !builderObject) return nullptr;

            jclass builderClass = env->GetObjectClass(builderObject);
            jmethodID createMethod = env->GetMethodID(builderClass, "create", "()Landroid/app/AlertDialog;");
            
            jobject dialog = env->CallObjectMethod(builderObject, createMethod);
            
            env->DeleteLocalRef(builderClass);
            
            AlertDialog* alertDialog = new AlertDialog();
            alertDialog->dialogObject = dialog;
            alertDialog->positiveCallback = positiveCallback;
            alertDialog->negativeCallback = negativeCallback;
            
            return alertDialog;
        }

        void show() {
            AlertDialog* dialog = create();
            if (dialog) {
                dialog->show();
            }
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

    void show() {
        JNIEnv* env = getJNIEnv();
        if (!env || !dialogObject) return;

        jclass dialogClass = env->GetObjectClass(dialogObject);
        jmethodID showMethod = env->GetMethodID(dialogClass, "show", "()V");
        
        env->CallVoidMethod(dialogObject, showMethod);
        env->DeleteLocalRef(dialogClass);
    }

    void dismiss() {
        JNIEnv* env = getJNIEnv();
        if (!env || !dialogObject) return;

        jclass dialogClass = env->GetObjectClass(dialogObject);
        jmethodID dismissMethod = env->GetMethodID(dialogClass, "dismiss", "()V");
        
        env->CallVoidMethod(dialogObject, dismissMethod);
        env->DeleteLocalRef(dialogClass);
    }

private:
    AlertDialog() : dialogObject(nullptr), positiveCallback(nullptr), negativeCallback(nullptr) {}

    static JNIEnv* getJNIEnv() {
        static JNIEnv* env = nullptr;
        return env;
    }

    static jobject getAndroidContext() {
        static jobject context = nullptr;
        return context;
    }
};

} // namespace aero

#endif // AERO_DIALOG_H
