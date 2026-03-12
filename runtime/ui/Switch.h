#ifndef AERO_SWITCH_H
#define AERO_SWITCH_H

#include "Button.h"
#include <jni.h>
#include <functional>

namespace aero {

class Switch : public Button {
private:
    std::function<void(bool)> changeListener;

public:
    Switch() : Button(""), changeListener(nullptr) {
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass switchClass = env->FindClass("android/widget/Switch");
        jmethodID constructor = env->GetMethodID(switchClass, "<init>", "(Landroid/content/Context;)V");
        
        jobject context = getAndroidContext();
        viewObject = env->NewObject(switchClass, constructor, context);
        
        env->DeleteLocalRef(switchClass);
    }

    void setChecked(bool checked) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass switchClass = env->GetObjectClass(viewObject);
        jmethodID setCheckedMethod = env->GetMethodID(switchClass, "setChecked", "(Z)V");
        
        env->CallVoidMethod(viewObject, setCheckedMethod, checked ? JNI_TRUE : JNI_FALSE);
        env->DeleteLocalRef(switchClass);
    }

    bool isChecked() const {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return false;

        jclass switchClass = env->GetObjectClass(viewObject);
        jmethodID isCheckedMethod = env->GetMethodID(switchClass, "isChecked", "()Z");
        
        jboolean checked = env->CallBooleanMethod(viewObject, isCheckedMethod);
        env->DeleteLocalRef(switchClass);
        
        return checked == JNI_TRUE;
    }

    void setOn CheckedChangeListener(std::function<void(bool)> listener) {
        changeListener = listener;
    }

    void toggle() {
        setChecked(!isChecked());
    }
};

} // namespace aero

#endif // AERO_SWITCH_H
