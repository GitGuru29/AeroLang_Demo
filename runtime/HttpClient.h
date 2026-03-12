#pragma once

#include <string>
#include <map>
#include <functional>
#include <memory>
#include <jni.h>
#include "Response.h"

namespace aero {

// Forward declaration
class JSONObject;

/**
 * HttpClient for making HTTP requests.
 * Supports GET, POST, PUT, DELETE with sync and async modes.
 * 
 * Usage:
 *   HttpClient client;
 *   client.setHeader("Authorization", "Bearer token123");
 *   
 *   // Sync request
 *   Response response = client.get("https://api.example.com/users");
 *   
 *   // Async request
 *   client.getAsync("https://api.example.com/users", [](Response res) {
 *       if (res.isSuccess()) {
 *           // Handle response
 *       }
 *   });
 */
class HttpClient {
public:
    HttpClient();
    HttpClient(JNIEnv* env);
    ~HttpClient();
    
    // Configuration (chainable)
    HttpClient& setHeader(const std::string& name, const std::string& value);
    HttpClient& setHeaders(const std::map<std::string, std::string>& headers);
    HttpClient& setTimeout(int timeoutMs);
    HttpClient& setConnectTimeout(int timeoutMs);
    HttpClient& setReadTimeout(int timeoutMs);
    HttpClient& setFollowRedirects(bool follow);
    
    // Clear headers for reuse
    void clearHeaders();
    
    // Synchronous requests (blocks until complete)
    Response get(const std::string& url);
    Response post(const std::string& url, const std::string& body);
    Response post(const std::string& url, const JSONObject& json);
    Response put(const std::string& url, const std::string& body);
    Response put(const std::string& url, const JSONObject& json);
    Response patch(const std::string& url, const std::string& body);
    Response delete_(const std::string& url);
    Response head(const std::string& url);
    
    // Asynchronous requests (returns immediately, calls callback on completion)
    void getAsync(const std::string& url, HttpCallback callback);
    void postAsync(const std::string& url, const std::string& body, HttpCallback callback);
    void postAsync(const std::string& url, const JSONObject& json, HttpCallback callback);
    void putAsync(const std::string& url, const std::string& body, HttpCallback callback);
    void deleteAsync(const std::string& url, HttpCallback callback);
    
    // Cancel pending async requests
    void cancelAll();
    
    // Common content types
    static constexpr const char* CONTENT_TYPE_JSON = "application/json";
    static constexpr const char* CONTENT_TYPE_FORM = "application/x-www-form-urlencoded";
    static constexpr const char* CONTENT_TYPE_MULTIPART = "multipart/form-data";
    
    // Initialize JNI (called by Activity)
    void initJNI(JNIEnv* env);
    
private:
    JNIEnv* env;
    std::map<std::string, std::string> defaultHeaders;
    int connectTimeout;
    int readTimeout;
    bool followRedirects;
    
    Response executeRequest(const std::string& method, const std::string& url, 
                            const std::string& body = "");
    void executeRequestAsync(const std::string& method, const std::string& url,
                             const std::string& body, HttpCallback callback);
    
    static JNIEnv* getDefaultEnv();
};

/**
 * Builder pattern for complex requests.
 */
class RequestBuilder {
public:
    RequestBuilder(const std::string& url);
    
    RequestBuilder& method(const std::string& method);
    RequestBuilder& header(const std::string& name, const std::string& value);
    RequestBuilder& body(const std::string& body);
    RequestBuilder& json(const JSONObject& json);
    RequestBuilder& timeout(int ms);
    
    Response execute();
    void executeAsync(HttpCallback callback);
    
private:
    std::string url;
    std::string httpMethod;
    std::string requestBody;
    std::map<std::string, std::string> headers;
    int timeoutMs;
};

} // namespace aero
