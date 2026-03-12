#ifndef AERO_CARD_VIEW_H
#define AERO_CARD_VIEW_H

#include "View.h"
#include <jni.h>

namespace aero {

class CardView : public View {
public:
    CardView() {
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass cardClass = env->FindClass("androidx/cardview/widget/CardView");
        jmethodID constructor = env->GetMethodID(cardClass, "<init>", "(Landroid/content/Context;)V");
        
        jobject context = getAndroidContext();
        viewObject = env->NewObject(cardClass, constructor, context);
        
        env->DeleteLocalRef(cardClass);
    }

    void setRadius(float radius) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass cardClass = env->GetObjectClass(viewObject);
        jmethodID setRadiusMethod = env->GetMethodID(cardClass, "setRadius", "(F)V");
        
        env->CallVoidMethod(viewObject, setRadiusMethod, radius);
        env->DeleteLocalRef(cardClass);
    }

    void setElevation(float elevation) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass cardClass = env->GetObjectClass(viewObject);
        jmethodID setCardElevationMethod = env->GetMethodID(cardClass, "setCardElevation", "(F)V");
        
        env->CallVoidMethod(viewObject, setCardElevationMethod, elevation);
        env->DeleteLocalRef(cardClass);
    }

    void setCardBackgroundColor(int color) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass cardClass = env->GetObjectClass(viewObject);
        jmethodID setCardBackgroundColorMethod = env->GetMethodID(cardClass, "setCardBackgroundColor", "(I)V");
        
        env->CallVoidMethod(viewObject, setCardBackgroundColorMethod, color);
        env->DeleteLocalRef(cardClass);
    }

    void addView(View* child) {
        if (!child) return;
        
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass cardClass = env->GetObjectClass(viewObject);
        jmethodID addViewMethod = env->GetMethodID(cardClass, "addView", "(Landroid/view/View;)V");
        
        env->CallVoidMethod(viewObject, addViewMethod, child->getViewObject());
        env->DeleteLocalRef(cardClass);
    }
};

} // namespace aero

#endif // AERO_CARD_VIEW_H
