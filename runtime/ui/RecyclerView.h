#ifndef AERO_RECYCLER_VIEW_H
#define AERO_RECYCLER_VIEW_H

#include "View.h"
#include "../collections/List.h"
#include <jni.h>
#include <functional>

namespace aero {

// Adapter base class for RecyclerView
template<typename T>
class RecyclerViewAdapter {
protected:
    List<T>* data;
    
public:
    RecyclerViewAdapter() : data(nullptr) {}
    virtual ~RecyclerViewAdapter() {}

    void setData(List<T>* newData) {
        data = newData;
    }

    virtual int getItemCount() const {
        return data ? data->size() : 0;
    }

    // Override these in subclass
    virtual jobject onCreateViewHolder(jobject parent, int viewType) = 0;
    virtual void onBindViewHolder(jobject holder, int position) = 0;
};

class RecyclerView : public View {
private:
    void* adapter;  // Generic adapter pointer

public:
    RecyclerView() : adapter(nullptr) {
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass recyclerViewClass = env->FindClass("androidx/recyclerview/widget/RecyclerView");
        jmethodID constructor = env->GetMethodID(recyclerViewClass, "<init>", "(Landroid/content/Context;)V");
        
        jobject context = getAndroidContext();
        viewObject = env->NewObject(recyclerViewClass, constructor, context);
        
        env->DeleteLocalRef(recyclerViewClass);
    }

    void setLayoutManager(int type) {
        // type: 0=Linear, 1=Grid, 2=StaggeredGrid
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass layoutManagerClass;
        if (type == 0) {
            layoutManagerClass = env->FindClass("androidx/recyclerview/widget/LinearLayoutManager");
        } else {
            layoutManagerClass = env->FindClass("androidx/recyclerview/widget/GridLayoutManager");
        }

        jmethodID constructor = env->GetMethodID(layoutManagerClass, "<init>", "(Landroid/content/Context;)V");
        jobject context = getAndroidContext();
        jobject layoutManager = env->NewObject(layoutManagerClass, constructor, context);

        jclass recyclerViewClass = env->GetObjectClass(viewObject);
        jmethodID setLayoutManagerMethod = env->GetMethodID(recyclerViewClass, "setLayoutManager", "(Landroidx/recyclerview/widget/RecyclerView$LayoutManager;)V");
        
        env->CallVoidMethod(viewObject, setLayoutManagerMethod, layoutManager);

        env->DeleteLocalRef(layoutManager);
        env->DeleteLocalRef(layoutManagerClass);
        env->DeleteLocalRef(recyclerViewClass);
    }

    template<typename T>
    void setAdapter(RecyclerViewAdapter<T>* newAdapter) {
        adapter = newAdapter;
        // In real implementation, would bridge to Java adapter
    }

    void notifyDataSetChanged() {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject || !adapter) return;

        // Get adapter from view and call notifyDataSetChanged
        jclass recyclerViewClass = env->GetObjectClass(viewObject);
        jmethodID getAdapterMethod = env->GetMethodID(recyclerViewClass, "getAdapter", "()Landroidx/recyclerview/widget/RecyclerView$Adapter;");
        jobject adapterObj = env->CallObjectMethod(viewObject, getAdapterMethod);

        if (adapterObj) {
            jclass adapterClass = env->GetObjectClass(adapterObj);
            jmethodID notifyMethod = env->GetMethodID(adapterClass, "notifyDataSetChanged", "()V");
            env->CallVoidMethod(adapterObj, notifyMethod);
            
            env->DeleteLocalRef(adapterClass);
            env->DeleteLocalRef(adapterObj);
        }

        env->DeleteLocalRef(recyclerViewClass);
    }
};

} // namespace aero

#endif // AERO_RECYCLER_VIEW_H
