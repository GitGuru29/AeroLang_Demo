#pragma once

#include <jni.h>
#include <cstdint>
#include <string>

namespace aero {

// View base class
class View {
public:
    View(JNIEnv* env, jobject viewInstance);
    virtual ~View();
    
    void setVisibility(bool visible);
    void setEnabled(bool enabled);
    
protected:
    // Internal JNI bridge members omitted.
};

// TextView
class TextView : public View {
public:
    TextView(JNIEnv* env, jobject viewInstance);
    
    void setText(const std::string& text);
    std::string getText() const;
    void setTextSize(float size);
    void setTextColor(int32_t color);
};

// Button
class Button : public TextView {
public:
    Button(JNIEnv* env, jobject viewInstance);
    
    // Button-specific methods can be added here
};

// Helper to find views
template<typename T>
T* findViewById(JNIEnv* env, jobject activity, int id);

} // namespace aero
