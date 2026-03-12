#pragma once

#include <string>
#include <map>
#include <vector>
#include <jni.h>

namespace aero {

// Forward declaration
class JSONObject;

/**
 * Response represents an HTTP response.
 * Contains status code, headers, and body.
 * 
 * Usage:
 *   Response response = client.get("https://api.example.com/users");
 *   if (response.isSuccess()) {
 *       JSONObject data = response.json();
 *       std::string name = data.getString("name");
 *   }
 */
class Response {
public:
    Response();
    Response(int statusCode, const std::string& body, 
             const std::map<std::string, std::string>& headers);
    Response(const Response& other);
    Response& operator=(const Response& other);
    ~Response();
    
    // Status
    int getStatusCode() const { return statusCode; }
    bool isSuccess() const { return statusCode >= 200 && statusCode < 300; }
    bool isRedirect() const { return statusCode >= 300 && statusCode < 400; }
    bool isClientError() const { return statusCode >= 400 && statusCode < 500; }
    bool isServerError() const { return statusCode >= 500; }
    
    // Body
    std::string getBody() const { return body; }
    std::vector<uint8_t> getBodyBytes() const;
    
    // Parse body as JSON (throws if invalid JSON)
    JSONObject json() const;
    
    // Headers
    std::string getHeader(const std::string& name) const;
    std::map<std::string, std::string> getHeaders() const { return headers; }
    bool hasHeader(const std::string& name) const;
    
    // Content info
    std::string getContentType() const;
    long getContentLength() const;
    
    // Error handling
    bool hasError() const { return !errorMessage.empty(); }
    std::string getErrorMessage() const { return errorMessage; }
    void setError(const std::string& error) { errorMessage = error; }
    
    // Common status codes
    static constexpr int OK = 200;
    static constexpr int CREATED = 201;
    static constexpr int NO_CONTENT = 204;
    static constexpr int BAD_REQUEST = 400;
    static constexpr int UNAUTHORIZED = 401;
    static constexpr int FORBIDDEN = 403;
    static constexpr int NOT_FOUND = 404;
    static constexpr int INTERNAL_SERVER_ERROR = 500;
    
private:
    int statusCode;
    std::string body;
    std::map<std::string, std::string> headers;
    std::string errorMessage;
};

/**
 * Callback type for async HTTP requests.
 */
using HttpCallback = std::function<void(Response response)>;

} // namespace aero
