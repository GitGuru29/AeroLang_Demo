#pragma once

#include <string>
#include <vector>
#include <memory>
#include <jni.h>

namespace aero {

// Forward declaration
class JSONArray;

/**
 * JSON Object wrapper for Android's org.json.JSONObject.
 * Provides parsing and creation of JSON objects.
 * 
 * Usage:
 *   // Parse JSON string
 *   auto json = JSONObject::parse("{\"name\":\"John\",\"age\":30}");
 *   std::string name = json.getString("name");
 *   int age = json.getInt("age");
 *   
 *   // Create JSON
 *   JSONObject obj;
 *   obj.put("name", "John");
 *   obj.put("age", 30);
 *   std::string str = obj.toString();
 */
class JSONObject {
public:
    JSONObject();
    JSONObject(JNIEnv* env, jobject jsonInstance);
    JSONObject(const JSONObject& other);
    JSONObject& operator=(const JSONObject& other);
    ~JSONObject();
    
    // Parse JSON string
    static JSONObject parse(const std::string& jsonString);
    
    // Getters - throw if key doesn't exist or wrong type
    std::string getString(const std::string& key);
    int getInt(const std::string& key);
    long getLong(const std::string& key);
    double getDouble(const std::string& key);
    bool getBoolean(const std::string& key);
    JSONObject getJSONObject(const std::string& key);
    JSONArray getJSONArray(const std::string& key);
    
    // Optional getters - return default if key doesn't exist
    std::string optString(const std::string& key, const std::string& defaultValue = "");
    int optInt(const std::string& key, int defaultValue = 0);
    long optLong(const std::string& key, long defaultValue = 0L);
    double optDouble(const std::string& key, double defaultValue = 0.0);
    bool optBoolean(const std::string& key, bool defaultValue = false);
    
    // Check if key exists
    bool has(const std::string& key);
    bool isNull(const std::string& key);
    
    // Get all keys
    std::vector<std::string> keys();
    int length();
    
    // Setters - return *this for chaining
    JSONObject& put(const std::string& key, const std::string& value);
    JSONObject& put(const std::string& key, int value);
    JSONObject& put(const std::string& key, long value);
    JSONObject& put(const std::string& key, double value);
    JSONObject& put(const std::string& key, bool value);
    JSONObject& put(const std::string& key, const JSONObject& value);
    JSONObject& put(const std::string& key, const JSONArray& value);
    JSONObject& putNull(const std::string& key);
    
    // Remove key
    void remove(const std::string& key);
    
    // Convert to string
    std::string toString();
    std::string toString(int indentSpaces); // Pretty print
    
    // Access underlying JNI object
    jobject getJNIObject() const { return jsonInstance; }
    bool isValid() const { return jsonInstance != nullptr; }
    
private:
    JNIEnv* env;
    jobject jsonInstance;
    bool ownsInstance;
    
    static JNIEnv* getDefaultEnv();
};

/**
 * JSON Array wrapper for Android's org.json.JSONArray.
 */
class JSONArray {
public:
    JSONArray();
    JSONArray(JNIEnv* env, jobject arrayInstance);
    JSONArray(const JSONArray& other);
    JSONArray& operator=(const JSONArray& other);
    ~JSONArray();
    
    // Parse JSON array string
    static JSONArray parse(const std::string& jsonString);
    
    // Getters by index
    std::string getString(int index);
    int getInt(int index);
    long getLong(int index);
    double getDouble(int index);
    bool getBoolean(int index);
    JSONObject getJSONObject(int index);
    JSONArray getJSONArray(int index);
    
    // Optional getters
    std::string optString(int index, const std::string& defaultValue = "");
    int optInt(int index, int defaultValue = 0);
    
    // Size
    int length();
    bool isNull(int index);
    
    // Add values
    JSONArray& put(const std::string& value);
    JSONArray& put(int value);
    JSONArray& put(long value);
    JSONArray& put(double value);
    JSONArray& put(bool value);
    JSONArray& put(const JSONObject& value);
    JSONArray& put(const JSONArray& value);
    
    // Convert to string
    std::string toString();
    std::string toString(int indentSpaces);
    
    // Access underlying JNI object
    jobject getJNIObject() const { return arrayInstance; }
    bool isValid() const { return arrayInstance != nullptr; }
    
private:
    JNIEnv* env;
    jobject arrayInstance;
    bool ownsInstance;
    
    static JNIEnv* getDefaultEnv();
};

/**
 * Top-level JSON utility for generating structure generically.
 */
class JSON {
public:
    static JSONObject parse(const std::string& jsonString) {
        return JSONObject::parse(jsonString);
    }
    
    static std::string stringify(JSONObject& obj, int indentSpaces = -1) {
        if (indentSpaces >= 0) {
            return obj.toString(indentSpaces);
        }
        return obj.toString();
    }
    
    static std::string stringify(JSONArray& arr, int indentSpaces = -1) {
        if (indentSpaces >= 0) {
            return arr.toString(indentSpaces);
        }
        return arr.toString();
    }
};

} // namespace aero
