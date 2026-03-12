#pragma once

#include <string>
#include <vector>
#include <jni.h>
#include <cstdarg>

namespace aero {

// AeroLang String type (UTF-8 wrapper with JNI support)
class String {
public:
    String();
    String(const char* str);
    String(const std::string& str);
    String(const String& other);
    ~String();
    
    String& operator=(const String& other);
    String operator+(const String& other) const;
    bool operator==(const String& other) const;
    bool operator!=(const String& other) const;
    
    const char* c_str() const;
    std::string toStdString() const;
    size_t length() const;
    bool isEmpty() const;
    
    // New string operations
    std::vector<String> split(const String& delimiter) const;
    String trim() const;
    static String format(const char* fmt, ...);
    String substring(size_t start, size_t length = std::string::npos) const;
    String replace(const String& from, const String& to) const;
    bool startsWith(const String& prefix) const;
    bool endsWith(const String& suffix) const;
    
    // JNI conversion
    jstring toJString(JNIEnv* env) const;
    static String fromJString(JNIEnv* env, jstring jstr);
    
private:
    std::string data;
};

} // namespace aero
