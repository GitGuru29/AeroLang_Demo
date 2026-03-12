#ifndef AERO_SWIPE_REFRESH_LAYOUT_H
#define AERO_SWIPE_REFRESH_LAYOUT_H

#include "View.h"
#include <jni.h>
#include <functional>

namespace aero {

class SwipeRefreshLayout : public View {
private:
    std::function<void()> refreshListener;

public:
    SwipeRefreshLayout() : refreshListener(nullptr) {
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass swipeRefreshClass = env->FindClass("androidx/swiperefreshlayout/widget/SwipeRefreshLayout");
        jmethodID constructor = env->GetMethodID(swipeRefreshClass, "<init>", "(Landroid/content/Context;)V");
        
        jobject context = getAndroidContext();
        viewObject = env->NewObject(swipeRefreshClass, constructor, context);
        
        env->DeleteLocalRef(swipeRefreshClass);
    }

    void addView(View* child) {
        if (!child) return;
        
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass swipeRefreshClass = env->GetObjectClass(viewObject);
        jmethodID addViewMethod = env->GetMethodID(swipeRefreshClass, "addView", "(Landroid/view/View;)V");
        
        env->CallVoidMethod(viewObject, addViewMethod, child->getViewObject());
        env->DeleteLocalRef(swipeRefreshClass);
    }

    void setRefreshing(bool refreshing) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass swipeRefreshClass = env->GetObjectClass(viewObject);
        jmethodID setRefreshingMethod = env->GetMethodID(swipeRefreshClass, "setRefreshing", "(Z)V");
        
        env->CallVoidMethod(viewObject, setRefreshingMethod, refreshing ? JNI_TRUE : JNI_FALSE);
        env->DeleteLocalRef(swipeRefreshClass);
    }

    bool isRefreshing() const {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return false;

        jclass swipeRefreshClass = env->GetObjectClass(viewObject);
        jmethodID isRefreshingMethod = env->GetMethodID(swipeRefreshClass, "isRefreshing", "()Z");
        
        jboolean refreshing = env->CallBooleanMethod(viewObject, isRefreshingMethod);
        env->DeleteLocalRef(swipeRefreshClass);
        
        return refreshing == JNI_TRUE;
    }

    void setOnRefreshListener(std::function<void()> listener) {
        refreshListener = listener;
    }

    void onRefresh() {
        if (refreshListener) {
            refreshListener();
        }
    }

    void setColorSchemeColors(int color1, int color2, int color3) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass swipeRefreshClass = env->GetObjectClass(viewObject);
        jmethodID setColorSchemeColorsMethod = env->GetMethodID(swipeRefreshClass, "setColorSchemeColors", "([I)V");
        
        jintArray colors = env->NewIntArray(3);
        jint colorsArray[3] = {color1, color2, color3};
        env->SetIntArrayRegion(colors, 0, 3, colorsArray);
        
        env->CallVoidMethod(viewObject, setColorSchemeColorsMethod, colors);
        
        env->DeleteLocalRef(colors);
        env->DeleteLocalRef(swipeRefreshClass);
    }
};

} // namespace aero

#endif // AERO_SWIPE_REFRESH_LAYOUT_H
