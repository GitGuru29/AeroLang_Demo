#pragma once

#include <jni.h>
#include <string>
#include <memory>
#include <functional>

namespace aero {

// Forward declarations
class View;
class Intent;
class SharedPreferences;

/**
 * Activity - Base class for all AeroLang activities.
 * 
 * Wraps Android Activity via JNI for C++ access to lifecycle,
 * UI, navigation, and system services.
 */
class Activity {
public:
    Activity();
    virtual ~Activity();
    
    // Initialize JNI connection (called from native bridge)
    void initJNI(JNIEnv* env, jobject activityInstance);
    
    // Lifecycle methods (override in subclass)
    virtual void onCreate() {}
    virtual void onStart() {}
    virtual void onResume() {}
    virtual void onPause() {}
    virtual void onStop() {}
    virtual void onDestroy() {}
    
    // UI Methods
    void setContentView(int layoutId);
    void setContentView(View* view);
    View* findViewById(int id);
    
    // Toast messages
    void showToast(const std::string& message);
    void showToast(const std::string& message, bool longDuration);
    
    // Navigation
    void startActivity(Intent* intent);
    void startActivityForResult(Intent* intent, int requestCode);
    void finish();
    void finishWithResult(int resultCode);
    void finishWithResult(int resultCode, Intent* data);
    
    // Intent handling
    Intent* getIntent();
    void setResult(int resultCode);
    void setResult(int resultCode, Intent* data);
    
    // Resources
    std::string getString(int resId);
    int getColor(int resId);
    
    // System services
    SharedPreferences* getSharedPreferences(const std::string& name, int mode);
    
    // Context access
    jobject getContext() { return activityInstance; }
    JNIEnv* getEnv() { return env; }
    
protected:
    JNIEnv* env;
    jobject activityInstance;
    jclass activityClass;
};

/**
 * Global activity instance registry.
 * Allows the JNI bridge to find activity instances by class name.
 */
class ActivityRegistry {
public:
    using Factory = std::function<Activity*()>;
    
    static void registerActivity(const std::string& name, Factory factory);
    static Activity* createActivity(const std::string& name);
    static Activity* getCurrentActivity();
    static void setCurrentActivity(Activity* activity);
    
private:
    static Activity* currentActivity;
};

} // namespace aero

// ============================================================
// JNI Bridge Macros
// ============================================================

/**
 * AERO_ACTIVITY_CLASS - Declare an activity class.
 * This generates the class with proper lifecycle method declarations.
 */
#define AERO_ACTIVITY_CLASS(ClassName) \
    class ClassName : public aero::Activity

/**
 * AERO_REGISTER_ACTIVITY - Register activity with the runtime.
 * Called at the end of activity class definition.
 */
#define AERO_REGISTER_ACTIVITY(ClassName) \
    namespace { \
        struct ClassName##Registrar { \
            ClassName##Registrar() { \
                aero::ActivityRegistry::registerActivity(#ClassName, \
                    []() -> aero::Activity* { return new ClassName(); }); \
            } \
        }; \
        static ClassName##Registrar ClassName##_registrar; \
    }

// ============================================================
// JNI Native Methods (called from Java)
// ============================================================

extern "C" {

// Global JNI initialization
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved);

// Native activity lifecycle callbacks
// These are called from the Java NativeActivity bridge class

JNIEXPORT void JNICALL
Java_com_aerolang_runtime_AeroActivity_nativeOnCreate(JNIEnv* env, jobject thiz, jstring activityName);

JNIEXPORT void JNICALL
Java_com_aerolang_runtime_AeroActivity_nativeOnStart(JNIEnv* env, jobject thiz);

JNIEXPORT void JNICALL
Java_com_aerolang_runtime_AeroActivity_nativeOnResume(JNIEnv* env, jobject thiz);

JNIEXPORT void JNICALL
Java_com_aerolang_runtime_AeroActivity_nativeOnPause(JNIEnv* env, jobject thiz);

JNIEXPORT void JNICALL
Java_com_aerolang_runtime_AeroActivity_nativeOnStop(JNIEnv* env, jobject thiz);

JNIEXPORT void JNICALL
Java_com_aerolang_runtime_AeroActivity_nativeOnDestroy(JNIEnv* env, jobject thiz);

}
