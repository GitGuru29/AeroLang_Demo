#ifndef AERO_CONSTRAINT_LAYOUT_H
#define AERO_CONSTRAINT_LAYOUT_H

#include "View.h"
#include <jni.h>
#include <vector>

namespace aero {

enum class ConstraintSide {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
    START,
    END
};

class ConstraintLayout : public View {
private:
    std::vector<View*> children;

public:
    ConstraintLayout() {
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass layoutClass = env->FindClass("androidx/constraintlayout/widget/ConstraintLayout");
        jmethodID constructor = env->GetMethodID(layoutClass, "<init>", "(Landroid/content/Context;)V");
        
        jobject context = getAndroidContext();
        viewObject = env->NewObject(layoutClass, constructor, context);
        
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

    void constrainTo(View* view, View* target, ConstraintSide viewSide, ConstraintSide targetSide, int margin = 0) {
        // Set up constraints programmatically
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass setClass = env->FindClass("androidx/constraintlayout/widget/ConstraintSet");
        jmethodID constructor = env->GetMethodID(setClass, "<init>", "()V");
        jobject constraintSet = env->NewObject(setClass, constructor);

        // Clone existing constraints
        jmethodID cloneMethod = env->GetMethodID(setClass, "clone", "(Landroidx/constraintlayout/widget/ConstraintLayout;)V");
        env->CallVoidMethod(constraintSet, cloneMethod, viewObject);

        // Apply constraint
        jmethodID connectMethod = env->GetMethodID(setClass, "connect", "(IIIII)V");
        env->CallVoidMethod(constraintSet, connectMethod, 
            view->getViewId(), 
            static_cast<int>(viewSide),
            target->getViewId(),
            static_cast<int>(targetSide),
            margin);

        // Apply to layout
        jmethodID applyToMethod = env->GetMethodID(setClass, "applyTo", "(Landroidx/constraintlayout/widget/ConstraintLayout;)V");
        env->CallVoidMethod(constraintSet, applyToMethod, viewObject);

        env->DeleteLocalRef(constraintSet);
        env->DeleteLocalRef(setClass);
    }
};

} // namespace aero

#endif // AERO_CONSTRAINT_LAYOUT_H
