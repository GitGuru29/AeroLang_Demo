#ifndef AERO_SEEK_BAR_H
#define AERO_SEEK_BAR_H

#include "ProgressBar.h"
#include <jni.h>
#include <functional>

namespace aero {

class SeekBar : public ProgressBar {
private:
    std::function<void(int)> changeListener;

public:
    SeekBar() : changeListener(nullptr) {
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass seekBarClass = env->FindClass("android/widget/SeekBar");
        jmethodID constructor = env->GetMethodID(seekBarClass, "<init>", "(Landroid/content/Context;)V");
        
        jobject context = getAndroidContext();
        viewObject = env->NewObject(seekBarClass, constructor, context);
        
        env->DeleteLocalRef(seekBarClass);
    }

    void setOnSeekBarChangeListener(std::function<void(int)> listener) {
        changeListener = listener;
        // In real implementation, would set up JNI callback
    }

    void onProgressChanged(int progress) {
        if (changeListener) {
            changeListener(progress);
        }
    }
};

} // namespace aero

#endif // AERO_SEEK_BAR_H
