#ifndef AERO_TOOLBAR_H
#define AERO_TOOLBAR_H

#include "View.h"
#include <jni.h>
#include <string>
#include <functional>

namespace aero {

class Toolbar : public View {
private:
    std::function<void(int)> menuItemClickListener;

public:
    Toolbar() : menuItemClickListener(nullptr) {
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass toolbarClass = env->FindClass("androidx/appcompat/widget/Toolbar");
        jmethodID constructor = env->GetMethodID(toolbarClass, "<init>", "(Landroid/content/Context;)V");
        
        jobject context = getAndroidContext();
        viewObject = env->NewObject(toolbarClass, constructor, context);
        
        env->DeleteLocalRef(toolbarClass);
    }

    void setTitle(const char* title) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass toolbarClass = env->GetObjectClass(viewObject);
        jmethodID setTitleMethod = env->GetMethodID(toolbarClass, "setTitle", "(Ljava/lang/CharSequence;)V");
        
        jstring jtitle = env->NewStringUTF(title);
        env->CallVoidMethod(viewObject, setTitleMethod, jtitle);
        
        env->DeleteLocalRef(jtitle);
        env->DeleteLocalRef(toolbarClass);
    }

    void setSubtitle(const char* subtitle) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass toolbarClass = env->GetObjectClass(viewObject);
        jmethodID setSubtitleMethod = env->GetMethodID(toolbarClass, "setSubtitle", "(Ljava/lang/CharSequence;)V");
        
        jstring jsubtitle = env->NewStringUTF(subtitle);
        env->CallVoidMethod(viewObject, setSubtitleMethod, jsubtitle);
        
        env->DeleteLocalRef(jsubtitle);
        env->DeleteLocalRef(toolbarClass);
    }

    void setNavigationIcon(int resourceId) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass toolbarClass = env->GetObjectClass(viewObject);
        jmethodID setNavigationIconMethod = env->GetMethodID(toolbarClass, "setNavigationIcon", "(I)V");
        
        env->CallVoidMethod(viewObject, setNavigationIconMethod, resourceId);
        env->DeleteLocalRef(toolbarClass);
    }

    void setNavigationOnClickListener(std::function<void()> listener) {
        // Set up navigation click listener
    }

    void inflateMenu(int menuResourceId) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass toolbarClass = env->GetObjectClass(viewObject);
        jmethodID inflateMenuMethod = env->GetMethodID(toolbarClass, "inflateMenu", "(I)V");
        
        env->CallVoidMethod(viewObject, inflateMenuMethod, menuResourceId);
        env->DeleteLocalRef(toolbarClass);
    }

    void setOnMenuItemClickListener(std::function<void(int)> listener) {
        menuItemClickListener = listener;
    }

    void onMenuItemClick(int itemId) {
        if (menuItemClickListener) {
            menuItemClickListener(itemId);
        }
    }
};

} // namespace aero

#endif // AERO_TOOLBAR_H
