#ifndef AERO_VIEW_PAGER_H
#define AERO_VIEW_PAGER_H

#include "View.h"
#include <jni.h>
#include <vector>
#include <functional>

namespace aero {

class ViewPager : public View {
private:
    std::vector<View*> pages;
    std::function<void(int)> pageChangeListener;

public:
    ViewPager() : pageChangeListener(nullptr) {
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass viewPagerClass = env->FindClass("androidx/viewpager/widget/ViewPager");
        jmethodID constructor = env->GetMethodID(viewPagerClass, "<init>", "(Landroid/content/Context;)V");
        
        jobject context = getAndroidContext();
        viewObject = env->NewObject(viewPagerClass, constructor, context);
        
        env->DeleteLocalRef(viewPagerClass);
    }

    void addPage(View* page) {
        pages.push_back(page);
    }

    void setCurrentItem(int position) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass viewPagerClass = env->GetObjectClass(viewObject);
        jmethodID setCurrentItemMethod = env->GetMethodID(viewPagerClass, "setCurrentItem", "(I)V");
        
        env->CallVoidMethod(viewObject, setCurrentItemMethod, position);
        env->DeleteLocalRef(viewPagerClass);
    }

    void setCurrentItem(int position, bool smoothScroll) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass viewPagerClass = env->GetObjectClass(viewObject);
        jmethodID setCurrentItemMethod = env->GetMethodID(viewPagerClass, "setCurrentItem", "(IZ)V");
        
        env->CallVoidMethod(viewObject, setCurrentItemMethod, position, smoothScroll ? JNI_TRUE : JNI_FALSE);
        env->DeleteLocalRef(viewPagerClass);
    }

    int getCurrentItem() const {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return 0;

        jclass viewPagerClass = env->GetObjectClass(viewObject);
        jmethodID getCurrentItemMethod = env->GetMethodID(viewPagerClass, "getCurrentItem", "()I");
        
        jint position = env->CallIntMethod(viewObject, getCurrentItemMethod);
        env->DeleteLocalRef(viewPagerClass);
        
        return static_cast<int>(position);
    }

    void setOnPageChangeListener(std::function<void(int)> listener) {
        pageChangeListener = listener;
        // In real implementation, set up JNI callback
    }

    void onPageChanged(int position) {
        if (pageChangeListener) {
            pageChangeListener(position);
        }
    }

    int getPageCount() const {
        return static_cast<int>(pages.size());
    }
};

} // namespace aero

#endif // AERO_VIEW_PAGER_H
