#ifndef AERO_LINEAR_LAYOUT_H
#define AERO_LINEAR_LAYOUT_H

#include "View.h"
#include <jni.h>
#include <vector>

namespace aero {

enum class Orientation {
    VERTICAL = 1,
    HORIZONTAL = 0
};

class LinearLayout : public View {
private:
    std::vector<View*> children;
    Orientation orientation;

public:
    LinearLayout(Orientation orient = Orientation::VERTICAL) : orientation(orient) {
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass layoutClass = env->FindClass("android/widget/LinearLayout");
        jmethodID constructor = env->GetMethodID(layoutClass, "<init>", "(Landroid/content/Context;)V");
        
        jobject context = getAndroidContext();
        viewObject = env->NewObject(layoutClass, constructor, context);
        
        // Set orientation
        jmethodID setOrientationMethod = env->GetMethodID(layoutClass, "setOrientation", "(I)V");
        env->CallVoidMethod(viewObject, setOrientationMethod, static_cast<int>(orient));
        
        env->DeleteLocalRef(layoutClass);
    }

    void addView(View* child) {
        if (!child) return;
        
        children.push_back(child);
        
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass layoutClass = env->GetObjectClass(viewObject);
        jmethodID addViewMethod = env->GetMethodID(layoutClass, "addView", "(Landroid/view/View;)V");
        
        env->CallVoidMethod(viewObject, addViewMethod, child->getViewObject());
        env->DeleteLocalRef(layoutClass);
    }

    void removeView(View* child) {
        auto it = std::find(children.begin(), children.end(), child);
        if (it != children.end()) {
            children.erase(it);
            
            JNIEnv* env = getJNIEnv();
            if (!env || !viewObject) return;

            jclass layoutClass = env->GetObjectClass(viewObject);
            jmethodID removeViewMethod = env->GetMethodID(layoutClass, "removeView", "(Landroid/view/View;)V");
            
            env->CallVoidMethod(viewObject, removeViewMethod, child->getViewObject());
            env->DeleteLocalRef(layoutClass);
        }
    }

    void removeAllViews() {
        children.clear();
        
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass layoutClass = env->GetObjectClass(viewObject);
        jmethodID removeAllViewsMethod = env->GetMethodID(layoutClass, "removeAllViews", "()V");
        
        env->CallVoidMethod(viewObject, removeAllViewsMethod);
        env->DeleteLocalRef(layoutClass);
    }

    int getChildCount() const {
        return static_cast<int>(children.size());
    }

    void setOrientation(Orientation orient) {
        orientation = orient;
        
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass layoutClass = env->GetObjectClass(viewObject);
        jmethodID setOrientationMethod = env->GetMethodID(layoutClass, "setOrientation", "(I)V");
        
        env->CallVoidMethod(viewObject, setOrientationMethod, static_cast<int>(orient));
        env->DeleteLocalRef(layoutClass);
    }

    void setPadding(int left, int top, int right, int bottom) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass viewClass = env->GetObjectClass(viewObject);
        jmethodID setPaddingMethod = env->GetMethodID(viewClass, "setPadding", "(IIII)V");
        
        env->CallVoidMethod(viewObject, setPaddingMethod, left, top, right, bottom);
        env->DeleteLocalRef(viewClass);
    }
};

} // namespace aero

#endif // AERO_LINEAR_LAYOUT_H
