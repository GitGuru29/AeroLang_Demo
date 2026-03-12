#ifndef AERO_TAB_LAYOUT_H
#define AERO_TAB_LAYOUT_H

#include "View.h"
#include "ViewPager.h"
#include <jni.h>
#include <string>
#include <functional>

namespace aero {

class TabLayout : public View {
private:
    std::function<void(int)> tabSelectListener;

public:
    TabLayout() : tabSelectListener(nullptr) {
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass tabLayoutClass = env->FindClass("com/google/android/material/tabs/TabLayout");
        jmethodID constructor = env->GetMethodID(tabLayoutClass, "<init>", "(Landroid/content/Context;)V");
        
        jobject context = getAndroidContext();
        viewObject = env->NewObject(tabLayoutClass, constructor, context);
        
        env->DeleteLocalRef(tabLayoutClass);
    }

    void addTab(const char* text) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass tabLayoutClass = env->GetObjectClass(viewObject);
        
        // Create new tab
        jmethodID newTabMethod = env->GetMethodID(tabLayoutClass, "newTab", "()Lcom/google/android/material/tabs/TabLayout$Tab;");
        jobject tab = env->CallObjectMethod(viewObject, newTabMethod);
        
        // Set tab text
        jclass tabClass = env->GetObjectClass(tab);
        jmethodID setTextMethod = env->GetMethodID(tabClass, "setText", "(Ljava/lang/CharSequence;)Lcom/google/android/material/tabs/TabLayout$Tab;");
        jstring jtext = env->NewStringUTF(text);
        env->CallObjectMethod(tab, setTextMethod, jtext);
        
        // Add tab
        jmethodID addTabMethod = env->GetMethodID(tabLayoutClass, "addTab", "(Lcom/google/android/material/tabs/TabLayout$Tab;)V");
        env->CallVoidMethod(viewObject, addTabMethod, tab);
        
        env->DeleteLocalRef(jtext);
        env->DeleteLocalRef(tabClass);
        env->DeleteLocalRef(tab);
        env->DeleteLocalRef(tabLayoutClass);
    }

    void setupWithViewPager(ViewPager* viewPager) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject || !viewPager) return;

        jclass tabLayoutClass = env->GetObjectClass(viewObject);
        jmethodID setupMethod = env->GetMethodID(tabLayoutClass, "setupWithViewPager", "(Landroidx/viewpager/widget/ViewPager;)V");
        
        env->CallVoidMethod(viewObject, setupMethod, viewPager->getViewObject());
        env->DeleteLocalRef(tabLayoutClass);
    }

    void setOnTabSelectedListener(std::function<void(int)> listener) {
        tabSelectListener = listener;
    }

    void onTabSelected(int position) {
        if (tabSelectListener) {
            tabSelectListener(position);
        }
    }

    int getSelectedTabPosition() const {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return 0;

        jclass tabLayoutClass = env->GetObjectClass(viewObject);
        jmethodID getSelectedTabPositionMethod = env->GetMethodID(tabLayoutClass, "getSelectedTabPosition", "()I");
        
        jint position = env->CallIntMethod(viewObject, getSelectedTabPositionMethod);
        env->DeleteLocalRef(tabLayoutClass);
        
        return static_cast<int>(position);
    }
};

} // namespace aero

#endif // AERO_TAB_LAYOUT_H
