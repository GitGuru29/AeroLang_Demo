#pragma once

#include <string>
#include <cstdint>
#include <jni.h>

namespace aero {

// Forward declaration
class Activity;

// Intent class for activity navigation and data passing
class Intent {
public:
    // Explicit intent (navigate to specific activity)
    Intent(JNIEnv* env, jobject context, const char* targetClassName);
    
    // Implicit intent (action-based)
    Intent(JNIEnv* env, const char* action);
    
    // Create from existing Java Intent object
    Intent(JNIEnv* env, jobject intentInstance);
    
    ~Intent();
    
    // Data passing methods - put extras
    void putExtra(const char* key, int32_t value);
    void putExtra(const char* key, int64_t value);
    void putExtra(const char* key, float value);
    void putExtra(const char* key, double value);
    void putExtra(const char* key, bool value);
    void putExtra(const char* key, const char* value);
    void putExtra(const char* key, const std::string& value);
    
    // Data retrieval methods - get extras
    int32_t getIntExtra(const char* key, int32_t defaultValue) const;
    int64_t getLongExtra(const char* key, int64_t defaultValue) const;
    float getFloatExtra(const char* key, float defaultValue) const;
    double getDoubleExtra(const char* key, double defaultValue) const;
    bool getBooleanExtra(const char* key, bool defaultValue) const;
    std::string getStringExtra(const char* key, const char* defaultValue = "") const;
    
    // Check if extra exists
    bool hasExtra(const char* key) const;
    
    // Set action for implicit intents
    void setAction(const char* action);
    std::string getAction() const;
    
    // Set data URI
    void setData(const char* uri);
    std::string getData() const;
    
    // Set type (MIME type)
    void setType(const char* mimeType);
    std::string getType() const;
    
    // Set flags
    void addFlags(int32_t flags);
    void setFlags(int32_t flags);
    int32_t getFlags() const;
    
    // Common Android Intent Actions
    static constexpr const char* ACTION_VIEW = "android.intent.action.VIEW";
    static constexpr const char* ACTION_SEND = "android.intent.action.SEND";
    static constexpr const char* ACTION_SENDTO = "android.intent.action.SENDTO";
    static constexpr const char* ACTION_DIAL = "android.intent.action.DIAL";
    static constexpr const char* ACTION_CALL = "android.intent.action.CALL";
    static constexpr const char* ACTION_EDIT = "android.intent.action.EDIT";
    static constexpr const char* ACTION_PICK = "android.intent.action.PICK";
    
    // Common Intent Flags
    static constexpr int32_t FLAG_ACTIVITY_NEW_TASK = 0x10000000;
    static constexpr int32_t FLAG_ACTIVITY_CLEAR_TOP = 0x04000000;
    static constexpr int32_t FLAG_ACTIVITY_SINGLE_TOP = 0x20000000;
    static constexpr int32_t FLAG_ACTIVITY_CLEAR_TASK = 0x00008000;
    static constexpr int32_t FLAG_ACTIVITY_NO_HISTORY = 0x40000000;
    
    // Get the underlying JNI Intent object
    jobject getJNIIntent() const { return intentInstance; }
    
private:
    // Internal JNI instance management omitted due to high-security proprietary details.
};

} // namespace aero
