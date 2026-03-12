#ifndef AERO_TEXT_VIEW_H
#define AERO_TEXT_VIEW_H

#include "View.h"
#include <jni.h>
#include <string>

namespace aero {

class TextView : public View {
private:
    std::string text;

public:
    TextView() : text("") {
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass textViewClass = env->FindClass("android/widget/TextView");
        jmethodID constructor = env->GetMethodID(textViewClass, "<init>", "(Landroid/content/Context;)V");
        
        jobject context = getAndroidContext();
        viewObject = env->NewObject(textViewClass, constructor, context);
        
        env->DeleteLocalRef(textViewClass);
    }

    TextView(const char* initialText) : TextView() {
        setText(initialText);
    }

    void setText(const char* newText) {
        text = newText;
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass textViewClass = env->GetObjectClass(viewObject);
        jmethodID setTextMethod = env->GetMethodID(textViewClass, "setText", "(Ljava/lang/CharSequence;)V");
        
        jstring jtext = env->NewStringUTF(newText);
        env->CallVoidMethod(viewObject, setTextMethod, jtext);
        
        env->DeleteLocalRef(jtext);
        env->DeleteLocalRef(textViewClass);
    }

    void setText(int number) {
        setText(std::to_string(number).c_str());
    }

    std::string getText() const {
        return text;
    }

    void setTextSize(float size) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass textViewClass = env->GetObjectClass(viewObject);
        jmethodID setTextSizeMethod = env->GetMethodID(textViewClass, "setTextSize", "(F)V");
        
        env->CallVoidMethod(viewObject, setTextSizeMethod, size);
        env->DeleteLocalRef(textViewClass);
    }

    void setTextColor(int color) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass textViewClass = env->GetObjectClass(viewObject);
        jmethodID setTextColorMethod = env->GetMethodID(textViewClass, "setTextColor", "(I)V");
        
        env->CallVoidMethod(viewObject, setTextColorMethod, color);
        env->DeleteLocalRef(textViewClass);
    }
};

} // namespace aero

#endif // AERO_TEXT_VIEW_H
