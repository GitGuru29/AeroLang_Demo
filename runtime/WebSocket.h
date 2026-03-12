#pragma once

#include <string>
#include <functional>
#include <memory>
#include <jni.h>

namespace aero {

/**
 * WebSocket for real-time bidirectional communication.
 * 
 * Usage:
 *   WebSocket ws;
 *   
 *   ws.onOpen([](){ 
 *       std::cout << "Connected!" << std::endl; 
 *   });
 *   
 *   ws.onMessage([](const std::string& msg) {
 *       std::cout << "Received: " << msg << std::endl;
 *   });
 *   
 *   ws.onClose([](int code, const std::string& reason) {
 *       std::cout << "Closed: " << reason << std::endl;
 *   });
 *   
 *   ws.onError([](const std::string& error) {
 *       std::cerr << "Error: " << error << std::endl;
 *   });
 *   
 *   ws.connect("wss://echo.websocket.org");
 *   ws.send("Hello WebSocket!");
 */
class WebSocket {
public:
    // Callback types
    using OpenCallback = std::function<void()>;
    using MessageCallback = std::function<void(const std::string& message)>;
    using BinaryCallback = std::function<void(const std::vector<uint8_t>& data)>;
    using CloseCallback = std::function<void(int code, const std::string& reason)>;
    using ErrorCallback = std::function<void(const std::string& error)>;
    
    // Connection states
    enum State {
        CONNECTING,
        OPEN,
        CLOSING,
        CLOSED
    };
    
    WebSocket();
    WebSocket(JNIEnv* env);
    ~WebSocket();
    
    /**
     * Connect to a WebSocket server.
     * @param url WebSocket URL (ws:// or wss://)
     */
    void connect(const std::string& url);
    
    /**
     * Connect with custom headers.
     */
    void connect(const std::string& url, 
                 const std::map<std::string, std::string>& headers);
    
    /**
     * Send text message.
     */
    void send(const std::string& message);
    
    /**
     * Send binary data.
     */
    void send(const std::vector<uint8_t>& data);
    
    /**
     * Send ping frame.
     */
    void ping();
    
    /**
     * Close the connection.
     */
    void close();
    
    /**
     * Close with code and reason.
     */
    void close(int code, const std::string& reason);
    
    /**
     * Get current connection state.
     */
    State getState() const { return state; }
    
    /**
     * Check if connected.
     */
    bool isConnected() const { return state == OPEN; }
    
    // Event handlers
    void onOpen(OpenCallback callback) { openCallback = callback; }
    void onMessage(MessageCallback callback) { messageCallback = callback; }
    void onBinary(BinaryCallback callback) { binaryCallback = callback; }
    void onClose(CloseCallback callback) { closeCallback = callback; }
    void onError(ErrorCallback callback) { errorCallback = callback; }
    
    // Close codes (RFC 6455)
    static constexpr int CLOSE_NORMAL = 1000;
    static constexpr int CLOSE_GOING_AWAY = 1001;
    static constexpr int CLOSE_PROTOCOL_ERROR = 1002;
    static constexpr int CLOSE_UNSUPPORTED = 1003;
    static constexpr int CLOSE_NO_STATUS = 1005;
    static constexpr int CLOSE_ABNORMAL = 1006;
    static constexpr int CLOSE_INVALID_DATA = 1007;
    static constexpr int CLOSE_POLICY_VIOLATION = 1008;
    static constexpr int CLOSE_MESSAGE_TOO_BIG = 1009;
    static constexpr int CLOSE_EXTENSION_REQUIRED = 1010;
    static constexpr int CLOSE_INTERNAL_ERROR = 1011;
    
    // Initialize JNI
    static void initJNI(JNIEnv* env);
    
private:
    JNIEnv* env;
    jobject webSocketInstance;
    State state;
    
    OpenCallback openCallback;
    MessageCallback messageCallback;
    BinaryCallback binaryCallback;
    CloseCallback closeCallback;
    ErrorCallback errorCallback;
    
    void runEventLoop();
    static JNIEnv* getDefaultEnv();
};

} // namespace aero
