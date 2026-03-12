#ifndef AERO_EDIT_TEXT_H
#define AERO_EDIT_TEXT_H

#include "TextView.h"
#include <jni.h>
#include <string>

namespace aero {

class EditText : public TextView {
public:
    EditText() {
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass editTextClass = env->FindClass("android/widget/EditText");
        jmethodID constructor = env->GetMethodID(editTextClass, "<init>", "(Landroid/content/Context;)V");
        
        jobject context = getAndroidContext();
        viewObject = env->NewObject(editTextClass, constructor, context);
        
        env->DeleteLocalRef(editTextClass);
    }

    void setHint(const char* hintText) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass editTextClass = env->GetObjectClass(viewObject);
        jmethodID setHintMethod = env->GetMethodID(editTextClass, "setHint", "(Ljava/lang/CharSequence;)V");
        
        jstring jhint = env->NewStringUTF(hintText);
        env->CallVoidMethod(viewObject, setHintMethod, jhint);
        
        env->DeleteLocalRef(jhint);
        env->DeleteLocalRef(editTextClass);
    }

    std::string getText() const {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return "";

        jclass editTextClass = env->GetObjectClass(viewObject);
        jmethodID getTextMethod = env->GetMethodID(editTextClass, "getText", "()Landroid/text/Editable;");
        
        jobject editable = env->CallObjectMethod(viewObject, getTextMethod);
        if (!editable) {
            env->DeleteLocalRef(editTextClass);
            return "";
        }

        jmethodID toStringMethod = env->GetMethodID(env->GetObjectClass(editable), "toString", "()Ljava/lang/String;");
        jstring jtext = (jstring)env->CallObjectMethod(editable, toStringMethod);
        
        const char* ctext = env->GetStringUTFChars(jtext, nullptr);
        std::string result(ctext);
        
        env->ReleaseStringUTFChars(jtext, ctext);
        env->DeleteLocalRef(jtext);
        env->DeleteLocalRef(editable);
        env->DeleteLocalRef(editTextClass);
        
        return result;
    }

    void setInputType(int type) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass editTextClass = env->GetObjectClass(viewObject);
        jmethodID setInputTypeMethod = env->GetMethodID(editTextClass, "setInputType", "(I)V");
        
        env->CallVoidMethod(viewObject, setInputTypeMethod, type);
        env->DeleteLocalRef(editTextClass);
    }
};

} // namespace aero

#endif // AERO_EDIT_TEXT_H
