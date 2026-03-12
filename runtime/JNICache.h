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

    // Cached jclass and jmethodID references omitted due to high-security proprietary details.
    // This singleton eliminates JNI resolution overhead across the bridge.
private:
    JNICache() = default;
};

} // namespace aero
