#pragma once

#include <string>
#include <jni.h>
#include "Intent.h"

namespace aero {

// Forward declarations
class Activity;
class FragmentManager;

// Fragment base class for modular UI components
class Fragment {
public:
    Fragment();
    virtual ~Fragment();
    
    // Lifecycle methods (to be overridden)
    virtual void onCreate() {}
    virtual void onCreateView() {}
    virtual void onViewCreated() {}
    virtual void onStart() {}
    virtual void onResume() {}
    virtual void onPause() {}
    virtual void onStop() {}
    virtual void onDestroyView() {}
    virtual void onDestroy() {}
    
    // Fragment state
    bool isAdded() const;
    bool isVisible() const;
    bool isResumed() const;
    
    // Access to parent activity
    Activity* getActivity();
    
    // Arguments (for passing data to fragments)
    void setArguments(Intent* args);
    Intent* getArguments();
    
    // Fragment ID and tag
    int getId() const { return fragmentId; }
    std::string getTag() const { return fragmentTag; }
    
protected:
    JNIEnv* env;
    jobject fragmentInstance;
    Activity* parentActivity;
    Intent* arguments;
    
    int fragmentId;
    std::string fragmentTag;
    
    void initJNI(JNIEnv* env, jobject instance, Activity* activity);
    
    friend class FragmentManager;
};

// Macro for registering fragments
#define AERO_REGISTER_FRAGMENT(ClassName) \
    extern "C" JNIEXPORT void JNICALL \
    Java_com_aerolang_runtime_NativeFragment_nativeOnCreate( \
        JNIEnv* env, jobject instance) { \
        static ClassName fragment; \
        fragment.initJNI(env, instance, nullptr); \
        fragment.onCreate(); \
    }

} // namespace aero
