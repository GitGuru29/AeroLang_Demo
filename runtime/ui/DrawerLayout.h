#ifndef AERO_DRAWER_LAYOUT_H
#define AERO_DRAWER_LAYOUT_H

#include "View.h"
#include <jni.h>
#include <functional>

namespace aero {

class NavigationView : public View {
private:
    std::function<void(int)> itemSelectedListener;

public:
    NavigationView() : itemSelectedListener(nullptr) {
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass navClass = env->FindClass("com/google/android/material/navigation/NavigationView");
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

    void setNavigationItemSelectedListener(std::function<void(int)> listener) {
        itemSelectedListener = listener;
    }

    void onNavigationItemSelected(int itemId) {
        if (itemSelectedListener) {
            itemSelectedListener(itemId);
        }
    }
};

class DrawerLayout : public View {
public:
    DrawerLayout() {
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass drawerClass = env->FindClass("androidx/drawerlayout/widget/DrawerLayout");
        jmethodID constructor = env->GetMethodID(drawerClass, "<init>", "(Landroid/content/Context;)V");
        
        jobject context = getAndroidContext();
        viewObject = env->NewObject(drawerClass, constructor, context);
        
        env->DeleteLocalRef(drawerClass);
    }

    void addView(View* child) {
        if (!child) return;
        
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass drawerClass = env->GetObjectClass(viewObject);
        jmethodID addViewMethod = env->GetMethodID(drawerClass, "addView", "(Landroid/view/View;)V");
        
        env->CallVoidMethod(viewObject, addViewMethod, child->getViewObject());
        env->DeleteLocalRef(drawerClass);
    }

    void openDrawer(int gravity) {
        // gravity: START=0x00800003, END=0x00800005
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass drawerClass = env->GetObjectClass(viewObject);
        jmethodID openDrawerMethod = env->GetMethodID(drawerClass, "openDrawer", "(I)V");
        
        env->CallVoidMethod(viewObject, openDrawerMethod, gravity);
        env->DeleteLocalRef(drawerClass);
    }

    void closeDrawer(int gravity) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass drawerClass = env->GetObjectClass(viewObject);
        jmethodID closeDrawerMethod = env->GetMethodID(drawerClass, "closeDrawer", "(I)V");
        
        env->CallVoidMethod(viewObject, closeDrawerMethod, gravity);
        env->DeleteLocalRef(drawerClass);
    }

    bool isDrawerOpen(int gravity) const {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return false;

        jclass drawerClass = env->GetObjectClass(viewObject);
        jmethodID isDrawerOpenMethod = env->GetMethodID(drawerClass, "isDrawerOpen", "(I)Z");
        
        jboolean isOpen = env->CallBooleanMethod(viewObject, isDrawerOpenMethod, gravity);
        env->DeleteLocalRef(drawerClass);
        
        return isOpen == JNI_TRUE;
    }

    void setDrawerLockMode(int lockMode) {
        // LOCK_MODE_UNLOCKED=0, LOCK_MODE_LOCKED_CLOSED=1, LOCK_MODE_LOCKED_OPEN=2
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass drawerClass = env->GetObjectClass(viewObject);
        jmethodID setDrawerLockModeMethod = env->GetMethodID(drawerClass, "setDrawerLockMode", "(I)V");
        
        env->CallVoidMethod(viewObject, setDrawerLockModeMethod, lockMode);
        env->DeleteLocalRef(drawerClass);
    }
};

} // namespace aero

#endif // AERO_DRAWER_LAYOUT_H
