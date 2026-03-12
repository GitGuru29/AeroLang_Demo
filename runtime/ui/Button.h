#ifndef AERO_BUTTON_H
#define AERO_BUTTON_H

#include "View.h"
#include <jni.h>
#include <string>
#include <functional>

namespace aero {

class Button : public View {
private:
    std::string text;
    std::function<void()> clickListener;

public:
    Button(const char* buttonText) : text(buttonText), clickListener(nullptr) {
        // Create Android Button via JNI
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass buttonClass = env->FindClass("android/widget/Button");
        jmethodID constructor = env->GetMethodID(buttonClass, "<init>", "(Landroid/content/Context;)V");
        
        jobject context = getAndroidContext();
        viewObject = env->NewObject(buttonClass, constructor, context);
        
        // Set text
        setText(buttonText);
        
        env->DeleteLocalRef(buttonClass);
    }

    void setText(const char* newText) {
        text = newText;
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass buttonClass = env->GetObjectClass(viewObject);
        jmethodID setTextMethod = env->GetMethodID(buttonClass, "setText", "(Ljava/lang/CharSequence;)V");
        
        jstring jtext = env->NewStringUTF(newText);
        env->CallVoidMethod(viewObject, setTextMethod, jtext);
        
        env->DeleteLocalRef(jtext);
        env->DeleteLocalRef(buttonClass);
    }

    std::string getText() const {
        return text;
    }

    void setOnClickListener(std::function<void()> listener) {
        clickListener = listener;
        
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        // Create OnClickListener wrapper
        jclass listenerClass = env->FindClass("android/view/View$OnClickListener");
        // Note: In real implementation, we'd need to create a JNI proxy
        // For now, this is a simplified version
        
        env->DeleteLocalRef(listenerClass);
    }

    void performClick() {
        if (clickListener) {
            clickListener();
        }
    }

    // Java callback bridge
    static void onClickCallback(jlong nativePtr) {
        Button* button = reinterpret_cast<Button*>(nativePtr);
        if (button && button->clickListener) {
            button->clickListener();
        }
    }
};

} // namespace aero

#endif // AERO_BUTTON_H
