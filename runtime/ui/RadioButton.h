#ifndef AERO_RADIO_H
#define AERO_RADIO_H

#include "Button.h"
#include <jni.h>
#include <vector>
#include <functional>

namespace aero {

class RadioButton : public Button {
public:
    RadioButton(const char* text) : Button(text) {
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass radioClass = env->FindClass("android/widget/RadioButton");
        jmethodID constructor = env->GetMethodID(radioClass, "<init>", "(Landroid/content/Context;)V");
        
        jobject context = getAndroidContext();
        viewObject = env->NewObject(radioClass, constructor, context);
        
        setText(text);
        env->DeleteLocalRef(radioClass);
    }

    void setChecked(bool checked) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass radioClass = env->GetObjectClass(viewObject);
        jmethodID setCheckedMethod = env->GetMethodID(radioClass, "setChecked", "(Z)V");
        
        env->CallVoidMethod(viewObject, setCheckedMethod, checked ? JNI_TRUE : JNI_FALSE);
        env->DeleteLocalRef(radioClass);
    }

    bool isChecked() const {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return false;

        jclass radioClass = env->GetObjectClass(viewObject);
        jmethodID isCheckedMethod = env->GetMethodID(radioClass, "isChecked", "()Z");
        
        jboolean checked = env->CallBooleanMethod(viewObject, isCheckedMethod);
        env->DeleteLocalRef(radioClass);
        
        return checked == JNI_TRUE;
    }
};

class RadioGroup : public View {
private:
    std::vector<RadioButton*> buttons;
    std::function<void(int)> checkedChangeListener;

public:
    RadioGroup() : checkedChangeListener(nullptr) {
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass groupClass = env->FindClass("android/widget/RadioGroup");
        jmethodID constructor = env->GetMethodID(groupClass, "<init>", "(Landroid/content/Context;)V");
        
        jobject context = getAndroidContext();
        viewObject = env->NewObject(groupClass, constructor, context);
        
        env->DeleteLocalRef(groupClass);
    }

    void addRadioButton(const char* text) {
        RadioButton* button = new RadioButton(text);
        buttons.push_back(button);
        
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass groupClass = env->GetObjectClass(viewObject);
        jmethodID addViewMethod = env->GetMethodID(groupClass, "addView", "(Landroid/view/View;)V");
        
        env->CallVoidMethod(viewObject, addViewMethod, button->getViewObject());
        env->DeleteLocalRef(groupClass);
    }

    void addRadioButton(RadioButton* button) {
        buttons.push_back(button);
        
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass groupClass = env->GetObjectClass(viewObject);
        jmethodID addViewMethod = env->GetMethodID(groupClass, "addView", "(Landroid/view/View;)V");
        
        env->CallVoidMethod(viewObject, addViewMethod, button->getViewObject());
        env->DeleteLocalRef(groupClass);
    }

    void check(int index) {
        if (index >= 0 && index < buttons.size()) {
            buttons[index]->setChecked(true);
        }
    }

    int getCheckedRadioButtonId() const {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return -1;

        jclass groupClass = env->GetObjectClass(viewObject);
        jmethodID getCheckedRadioButtonIdMethod = env->GetMethodID(groupClass, "getCheckedRadioButtonId", "()I");
        
        jint id = env->CallIntMethod(viewObject, getCheckedRadioButtonIdMethod);
        env->DeleteLocalRef(groupClass);
        
        return static_cast<int>(id);
    }

    void setOnCheckedChangeListener(std::function<void(int)> listener) {
        checkedChangeListener = listener;
    }

    void onCheckedChanged(int checkedId) {
        if (checkedChangeListener) {
            checkedChangeListener(checkedId);
        }
    }
};

} // namespace aero

#endif // AERO_RADIO_H
