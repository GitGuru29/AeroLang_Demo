#ifndef AERO_SCROLL_VIEW_H
#define AERO_SCROLL_VIEW_H

#include "View.h"
#include <jni.h>

namespace aero {

class ScrollView : public View {
public:
    ScrollView() {
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass scrollViewClass = env->FindClass("android/widget/ScrollView");
        jmethodID constructor = env->GetMethodID(scrollViewClass, "<init>", "(Landroid/content/Context;)V");
        
        jobject context = getAndroidContext();
        viewObject = env->NewObject(scrollViewClass, constructor, context);
        
        env->DeleteLocalRef(scrollViewClass);
    }

    void addView(View* child) {
        if (!child) return;
        
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass scrollViewClass = env->GetObjectClass(viewObject);
        jmethodID addViewMethod = env->GetMethodID(scrollViewClass, "addView", "(Landroid/view/View;)V");
        
        env->CallVoidMethod(viewObject, addViewMethod, child->getViewObject());
        env->DeleteLocalRef(scrollViewClass);
    }

    void scrollTo(int x, int y) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass scrollViewClass = env->GetObjectClass(viewObject);
        jmethodID scrollToMethod = env->GetMethodID(scrollViewClass, "scrollTo", "(II)V");
        
        env->CallVoidMethod(viewObject, scrollToMethod, x, y);
        env->DeleteLocalRef(scrollViewClass);
    }

    void fullScroll(int direction) {
        // FOCUS_UP=33, FOCUS_DOWN=130
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass scrollViewClass = env->GetObjectClass(viewObject);
        jmethodID fullScrollMethod = env->GetMethodID(scrollViewClass, "fullScroll", "(I)V");
        
        env->CallVoidMethod(viewObject, fullScrollMethod, direction);
        env->DeleteLocalRef(scrollViewClass);
    }
};

} // namespace aero

#endif // AERO_SCROLL_VIEW_H
