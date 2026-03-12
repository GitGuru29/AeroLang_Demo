#pragma once

#include <jni.h>
#include <map>
#include <string>

namespace aero {

/**
 * aero::JNICache
 * Singleton for caching jclass definitions and jmethodIDs to eliminate
 * overhead created by repeated resolving across the JNI bridge.
 */
class JNICache {
public:
    static JNICache& getInstance() {
        static JNICache instance;
        return instance;
    }

    // Cache Initialization
    void init(JNIEnv* env);
    void release(JNIEnv* env);

    // Cached Data Structures
    
    // -- View --
    jclass viewClass;
    jmethodID viewSetVisibility;
    jmethodID viewSetEnabled;

    // -- TextView --
    jclass textViewClass;
    jmethodID textViewSetText;
    jmethodID textViewGetText;
    jmethodID textViewSetTextSize;
    jmethodID textViewSetTextColor;

    // -- Activity --
    jclass activityClass;
    jmethodID activityFindViewById;
    jmethodID activitySetContentView;
    jmethodID activityStartActivity;

private:
    JNICache() = default;
    ~JNICache() = default;
    JNICache(const JNICache&) = delete;
    JNICache& operator=(const JNICache&) = delete;
    
    bool initialized = false;
};

} // namespace aero
