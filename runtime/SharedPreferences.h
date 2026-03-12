#pragma once

#include <string>
#include <set>
#include <jni.h>

namespace aero {

// Forward declaration
class Activity;

/**
 * SharedPreferences wrapper for Android's SharedPreferences API.
 * Provides key-value storage that persists across app restarts.
 * 
 * Usage:
 *   auto prefs = activity->getSharedPreferences("settings", MODE_PRIVATE);
 *   auto editor = prefs->edit();
 *   editor.putString("username", "John");
 *   editor.putInt("score", 100);
 *   editor.apply();
 */
class SharedPreferences {
public:
    // Access modes
    static constexpr int MODE_PRIVATE = 0;
    static constexpr int MODE_WORLD_READABLE = 1;  // Deprecated in Android
    static constexpr int MODE_WORLD_WRITEABLE = 2; // Deprecated in Android
    
    SharedPreferences(JNIEnv* env, jobject prefsInstance);
    ~SharedPreferences();
    
    // Getters - return default value if key doesn't exist
    std::string getString(const std::string& key, const std::string& defaultValue);
    int getInt(const std::string& key, int defaultValue);
    long getLong(const std::string& key, long defaultValue);
    float getFloat(const std::string& key, float defaultValue);
    bool getBoolean(const std::string& key, bool defaultValue);
    std::set<std::string> getStringSet(const std::string& key);
    
    // Check if key exists
    bool contains(const std::string& key);
    
    // Get all keys
    std::set<std::string> getAll();
    
    /**
     * Editor class for modifying SharedPreferences.
     * Changes are batched until apply() or commit() is called.
     */
    class Editor {
    public:
        Editor(JNIEnv* env, jobject editorInstance);
        ~Editor();
        
        // Setters - return *this for chaining
        Editor& putString(const std::string& key, const std::string& value);
        Editor& putInt(const std::string& key, int value);
        Editor& putLong(const std::string& key, long value);
        Editor& putFloat(const std::string& key, float value);
        Editor& putBoolean(const std::string& key, bool value);
        Editor& putStringSet(const std::string& key, const std::set<std::string>& values);
        
        // Remove a key
        Editor& remove(const std::string& key);
        
        // Clear all preferences
        Editor& clear();
        
        // Save changes asynchronously (preferred)
        void apply();
        
        // Save changes synchronously, returns true on success
        bool commit();
        
    private:
        JNIEnv* env;
        jobject editorInstance;
    };
    
    // Get an Editor to modify preferences
    Editor edit();
    
private:
    JNIEnv* env;
    jobject prefsInstance;
};

} // namespace aero
