#ifndef AERO_IMAGE_VIEW_H
#define AERO_IMAGE_VIEW_H

#include "View.h"
#include <jni.h>
#include <string>

namespace aero {

class ImageView : public View {
public:
    ImageView() {
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass imageViewClass = env->FindClass("android/widget/ImageView");
        jmethodID constructor = env->GetMethodID(imageViewClass, "<init>", "(Landroid/content/Context;)V");
        
        jobject context = getAndroidContext();
        viewObject = env->NewObject(imageViewClass, constructor, context);
        
        env->DeleteLocalRef(imageViewClass);
    }

    void setImageResource(int resourceId) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass imageViewClass = env->GetObjectClass(viewObject);
        jmethodID setImageResourceMethod = env->GetMethodID(imageViewClass, "setImageResource", "(I)V");
        
        env->CallVoidMethod(viewObject, setImageResourceMethod, resourceId);
        env->DeleteLocalRef(imageViewClass);
    }

    void setScaleType(int scaleType) {
        // ScaleType: CENTER=1, CENTER_CROP=6, FIT_CENTER=3, etc.
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass scaleTypeClass = env->FindClass("android/widget/ImageView$ScaleType");
        jclass imageViewClass = env->GetObjectClass(viewObject);
        
        // Get scale type enum value
        jmethodID valuesMethod = env->GetStaticMethodID(scaleTypeClass, "values", "()[Landroid/widget/ImageView$ScaleType;");
        jobjectArray scaleTypes = (jobjectArray)env->CallStaticObjectMethod(scaleTypeClass, valuesMethod);
        jobject scaleTypeObj = env->GetObjectArrayElement(scaleTypes, scaleType);
        
        jmethodID setScaleTypeMethod = env->GetMethodID(imageViewClass, "setScaleType", "(Landroid/widget/ImageView$ScaleType;)V");
        env->CallVoidMethod(viewObject, setScaleTypeMethod, scaleTypeObj);
        
        env->DeleteLocalRef(scaleTypeObj);
        env->DeleteLocalRef(scaleTypes);
        env->DeleteLocalRef(scaleTypeClass);
        env->DeleteLocalRef(imageViewClass);
    }
};

} // namespace aero

#endif // AERO_IMAGE_VIEW_H
