#pragma once

#include <jni.h>
#include <string>
#include <functional>
#include <memory>

namespace aero {

// Forward declarations
class Intent;
class Activity;
class Service;

/**
 * BroadcastReceiver - Listens for system and custom broadcast events.
 * 
 * Usage in AeroLang:
 *   class NetworkReceiver {
 *       fun onReceive(intent: Intent) {
 *           val action = intent.getAction()
 *           if (action == BroadcastReceiver.ACTION_CONNECTIVITY_CHANGE) {
 *               showToast("Network changed!")
 *           }
 *       }
 *   }
 */
class BroadcastReceiver {
public:
    BroadcastReceiver();
    virtual ~BroadcastReceiver();
    
    // Override to handle broadcasts
    virtual void onReceive(JNIEnv* env, jobject context, Intent* intent) = 0;
    
    // Common system broadcast actions
    static constexpr const char* ACTION_BOOT_COMPLETED = "android.intent.action.BOOT_COMPLETED";
    static constexpr const char* ACTION_BATTERY_LOW = "android.intent.action.BATTERY_LOW";
    static constexpr const char* ACTION_BATTERY_OKAY = "android.intent.action.BATTERY_OKAY";
    static constexpr const char* ACTION_POWER_CONNECTED = "android.intent.action.ACTION_POWER_CONNECTED";
    static constexpr const char* ACTION_POWER_DISCONNECTED = "android.intent.action.ACTION_POWER_DISCONNECTED";
    static constexpr const char* ACTION_CONNECTIVITY_CHANGE = "android.net.conn.CONNECTIVITY_CHANGE";
    static constexpr const char* ACTION_AIRPLANE_MODE_CHANGED = "android.intent.action.AIRPLANE_MODE";
    static constexpr const char* ACTION_SCREEN_ON = "android.intent.action.SCREEN_ON";
    static constexpr const char* ACTION_SCREEN_OFF = "android.intent.action.SCREEN_OFF";
    static constexpr const char* ACTION_TIME_TICK = "android.intent.action.TIME_TICK";
    static constexpr const char* ACTION_TIMEZONE_CHANGED = "android.intent.action.TIMEZONE_CHANGED";
    static constexpr const char* ACTION_LOCALE_CHANGED = "android.intent.action.LOCALE_CHANGED";
    static constexpr const char* ACTION_PACKAGE_ADDED = "android.intent.action.PACKAGE_ADDED";
    static constexpr const char* ACTION_PACKAGE_REMOVED = "android.intent.action.PACKAGE_REMOVED";
};

/**
 * Lambda-based BroadcastReceiver for inline usage.
 */
class LambdaBroadcastReceiver : public BroadcastReceiver {
public:
    using Callback = std::function<void(JNIEnv*, jobject, Intent*)>;
    
    explicit LambdaBroadcastReceiver(Callback callback) : callback(callback) {}
    
    void onReceive(JNIEnv* env, jobject context, Intent* intent) override {
        if (callback) {
            callback(env, context, intent);
        }
    }
    
private:
    Callback callback;
};

/**
 * IntentFilter - Specifies which broadcast actions to receive.
 */
class IntentFilter {
public:
    IntentFilter();
    explicit IntentFilter(const std::string& action);
    ~IntentFilter();
    
    IntentFilter& addAction(const std::string& action);
    IntentFilter& addCategory(const std::string& category);
    IntentFilter& setPriority(int priority);
    
    // Build the JNI IntentFilter object
    jobject buildJNI(JNIEnv* env) const;
    
    const std::vector<std::string>& getActions() const { return actions; }
    
private:
    std::vector<std::string> actions;
    std::vector<std::string> categories;
    int priority;
};

/**
 * BroadcastManager - Register/unregister receivers and send broadcasts.
 * Typically accessed through Activity or Service.
 */
class BroadcastManager {
public:
    BroadcastManager();
    BroadcastManager(JNIEnv* env, jobject context);
    ~BroadcastManager();
    
    void initJNI(JNIEnv* env, jobject context);
    
    // Register a receiver for specific actions
    void registerReceiver(BroadcastReceiver* receiver, const IntentFilter& filter);
    void unregisterReceiver(BroadcastReceiver* receiver);
    
    // Send broadcasts
    void sendBroadcast(Intent* intent);
    void sendOrderedBroadcast(Intent* intent);
    
    // Local broadcasts (within app only, more efficient)
    void sendLocalBroadcast(Intent* intent);
    void registerLocalReceiver(BroadcastReceiver* receiver, const IntentFilter& filter);
    void unregisterLocalReceiver(BroadcastReceiver* receiver);
    
private:
    JNIEnv* env;
    jobject context;
};

/**
 * BroadcastReceiver Registry.
 */
class BroadcastReceiverRegistry {
public:
    using Factory = std::function<BroadcastReceiver*()>;
    
    static void registerReceiver(const std::string& name, Factory factory);
    static BroadcastReceiver* createReceiver(const std::string& name);
};

#define AERO_REGISTER_RECEIVER(ClassName) \
    namespace { \
        struct ClassName##ReceiverRegistrar { \
            ClassName##ReceiverRegistrar() { \
                aero::BroadcastReceiverRegistry::registerReceiver(#ClassName, \
                    []() -> aero::BroadcastReceiver* { return new ClassName(); }); \
            } \
        }; \
        static ClassName##ReceiverRegistrar ClassName##_receiver_registrar; \
    }

} // namespace aero
