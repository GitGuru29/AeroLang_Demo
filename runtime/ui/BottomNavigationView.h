#ifndef AERO_BOTTOM_NAV_H
#define AERO_BOTTOM_NAV_H

#include "View.h"
#include <jni.h>
#include <functional>

namespace aero {

class BottomNavigationView : public View {
private:
    std::function<void(int)> itemSelectedListener;

public:
    BottomNavigationView() : itemSelectedListener(nullptr) {
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass navClass = env->FindClass("com/google/android/material/bottomnavigation/BottomNavigationView");
        jmethodID constructor = env->GetMethodID(navClass, "<init>", "(Landroid/content/Context;)V");
        
        jobject context = getAndroidContext();
        viewObject = env->NewObject(navClass, constructor, context);
        
        env->DeleteLocalRef(navClass);
    }

    void inflateMenu(int menuResourceId) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass navClass = env->GetObjectClass(viewObject);
        jmethodID inflateMenuMethod = env->GetMethodID(navClass, "inflateMenu", "(I)V");
        
        env->CallVoidMethod(viewObject, inflateMenuMethod, menuResourceId);
        env->DeleteLocalRef(navClass);
    }

    void setSelectedItemId(int itemId) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass navClass = env->GetObjectClass(viewObject);
        jmethodID setSelectedItemIdMethod = env->GetMethodID(navClass, "setSelectedItemId", "(I)V");
        
        env->CallVoidMethod(viewObject, setSelectedItemIdMethod, itemId);
        env->DeleteLocalRef(navClass);
    }

    int getSelectedItemId() const {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return -1;

        jclass navClass = env->GetObjectClass(viewObject);
        jmethodID getSelectedItemIdMethod = env->GetMethodID(navClass, "getSelectedItemId", "()I");
        
        jint itemId = env->CallIntMethod(viewObject, getSelectedItemIdMethod);
        env->DeleteLocalRef(navClass);
        
        return static_cast<int>(itemId);
    }

    void setOnItemSelectedListener(std::function<void(int)> listener) {
        itemSelectedListener = listener;
        // Set up JNI callback
    }

    void onItemSelected(int itemId) {
        if (itemSelectedListener) {
            itemSelectedListener(itemId);
        }
    }

    void setItemIconTintList(int colorStateList) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass navClass = env->GetObjectClass(viewObject);
        jmethodID setItemIconTintListMethod = env->GetMethodID(navClass, "setItemIconTintList", "(Landroid/content/res/ColorStateList;)V");
        
        // In real implementation, create ColorStateList from resource
        env->CallVoidMethod(viewObject, setItemIconTintListMethod, nullptr);
        env->DeleteLocalRef(navClass);
    }
};

} // namespace aero

#endif // AERO_BOTTOM_NAV_H
