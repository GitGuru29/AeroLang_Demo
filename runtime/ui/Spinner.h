#ifndef AERO_SPINNER_H
#define AERO_SPINNER_H

#include "View.h"
#include "../collections/List.h"
#include <jni.h>
#include <functional>
#include <string>

namespace aero {

class Spinner : public View {
private:
    std::function<void(int)> itemSelectedListener;
    List<std::string> items;

public:
    Spinner() : itemSelectedListener(nullptr) {
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass spinnerClass = env->FindClass("android/widget/Spinner");
        jmethodID constructor = env->GetMethodID(spinnerClass, "<init>", "(Landroid/content/Context;)V");
        
        jobject context = getAndroidContext();
        viewObject = env->NewObject(spinnerClass, constructor, context);
        
        env->DeleteLocalRef(spinnerClass);
    }

    void setItems(List<std::string>* itemList) {
        items = *itemList;
        // Set up adapter with items
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        // Create ArrayAdapter
        jclass adapterClass = env->FindClass("android/widget/ArrayAdapter");
        jmethodID constructor = env->GetMethodID(adapterClass, "<init>", "(Landroid/content/Context;I)V");
        
        jobject context = getAndroidContext();
        jobject adapter = env->NewObject(adapterClass, constructor, context, android.R.layout.simple_spinner_item);

        // Add items
        jmethodID addMethod = env->GetMethodID(adapterClass, "add", "(Ljava/lang/Object;)V");
        for (int i = 0; i < items.size(); i++) {
            jstring jitem = env->NewStringUTF(items.get(i).c_str());
            env->CallVoidMethod(adapter, addMethod, jitem);
            env->DeleteLocalRef(jitem);
        }

        // Set adapter
        jclass spinnerClass = env->GetObjectClass(viewObject);
        jmethodID setAdapterMethod = env->GetMethodID(spinnerClass, "setAdapter", "(Landroid/widget/SpinnerAdapter;)V");
        env->CallVoidMethod(viewObject, setAdapterMethod, adapter);

        env->DeleteLocalRef(adapter);
        env->DeleteLocalRef(adapterClass);
        env->DeleteLocalRef(spinnerClass);
    }

    void setSelection(int position) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass spinnerClass = env->GetObjectClass(viewObject);
        jmethodID setSelectionMethod = env->GetMethodID(spinnerClass, "setSelection", "(I)V");
        
        env->CallVoidMethod(viewObject, setSelectionMethod, position);
        env->DeleteLocalRef(spinnerClass);
    }

    int getSelectedItemPosition() const {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return -1;

        jclass spinnerClass = env->GetObjectClass(viewObject);
        jmethodID getSelectedItemPositionMethod = env->GetMethodID(spinnerClass, "getSelectedItemPosition", "()I");
        
        jint position = env->CallIntMethod(viewObject, getSelectedItemPositionMethod);
        env->DeleteLocalRef(spinnerClass);
        
        return static_cast<int>(position);
    }

    void setOnItemSelectedListener(std::function<void(int)> listener) {
        itemSelectedListener = listener;
    }

    void onItemSelected(int position) {
        if (itemSelectedListener) {
            itemSelectedListener(position);
        }
    }
};

} // namespace aero

#endif // AERO_SPINNER_H
