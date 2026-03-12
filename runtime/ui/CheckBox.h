#ifndef AERO_CHECKBOX_H
#define AERO_CHECKBOX_H

#include "Button.h"
#include <jni.h>
#include <functional>

namespace aero {

class CheckBox : public Button {
private:
    std::function<void(bool)> changeListener;

public:
    CheckBox(const char* labelText) : Button(labelText), changeListener(nullptr) {
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass checkBoxClass = env->FindClass("android/widget/CheckBox");
        jmethodID constructor = env->GetMethodID(checkBoxClass, "<init>", "(Landroid/content/Context;)V");
        
        jobject context = getAndroidContext();
        viewObject = env->NewObject(checkBoxClass, constructor, context);
        
        setText(labelText);
        env->DeleteLocalRef(checkBoxClass);
    }

    void setChecked(bool checked) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass checkBoxClass = env->GetObjectClass(viewObject);
        jmethodID setCheckedMethod = env->GetMethodID(checkBoxClass, "setChecked", "(Z)V");
        
        env->CallVoidMethod(viewObject, setCheckedMethod, checked ? JNI_TRUE : JNI_FALSE);
        env->DeleteLocalRef(checkBoxClass);
    }

    bool isChecked() const {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return false;

        jclass checkBoxClass = env->GetObjectClass(viewObject);
        jmethodID isCheckedMethod = env->GetMethodID(checkBoxClass, "isChecked", "()Z");
        
        jboolean checked = env->CallBooleanMethod(viewObject, isCheckedMethod);
        env->DeleteLocalRef(checkBoxClass);
        
        return checked == JNI_TRUE;
    }

    void setOnCheckedChangeListener(std::function<void(bool)> listener) {
        changeListener = listener;
    }

    void toggle() {
        setChecked(!isChecked());
    }
};

} // namespace aero

#endif // AERO_CHECKBOX_H
