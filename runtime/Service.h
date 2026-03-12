#pragma once

#include <jni.h>
#include <string>
#include <memory>
#include <functional>

namespace aero {

// Forward declarations
class Intent;
class NotificationBuilder;

/**
 * Service - Base class for all AeroLang background services.
 * 
 * Wraps Android Service via JNI. Supports both started (startService)
 * and bound (bindService) modes.
 * 
 * Usage in AeroLang:
 *   service MusicService {
 *       fun onStartCommand(intent: Intent): Int {
 *           return START_STICKY
 *       }
 *       fun onDestroy() {
 *           // cleanup
 *       }
 *   }
 */
class Service {
public:
    Service();
    virtual ~Service();
    
    // Initialize JNI connection (called from native bridge)
    void initJNI(JNIEnv* env, jobject serviceInstance);
    
    // Lifecycle methods (override in subclass)
    virtual void onCreate() {}
    virtual int onStartCommand(Intent* intent, int flags, int startId) { return START_STICKY; }
    virtual void onDestroy() {}
    
    // Bind lifecycle (override for bound services)
    virtual bool onBind(Intent* intent) { return false; }
    virtual void onUnbind(Intent* intent) {}
    virtual void onRebind(Intent* intent) {}
    
    // Service control
    void stopSelf();
    void stopSelf(int startId);
    
    // Foreground service
    void startForeground(int notificationId, jobject notification);
    void stopForeground(bool removeNotification);
    
    // Context access
    jobject getContext() { return serviceInstance; }
    JNIEnv* getEnv() { return env; }
    
    // Start return values
    static constexpr int START_STICKY = 1;
    static constexpr int START_NOT_STICKY = 2;
    static constexpr int START_REDELIVER_INTENT = 3;
    static constexpr int START_STICKY_COMPATIBILITY = 0;
    
    // Service start flags
    static constexpr int START_FLAG_REDELIVERY = 0x0001;
    static constexpr int START_FLAG_RETRY = 0x0002;
    
protected:
    // Internal JNI bridge members and lifecycle management omitted.
};

/**
 * IntentService - Convenience class that handles async work.
 * Automatically stops after onHandleIntent completes.
 */
class IntentService : public Service {
public:
    IntentService();
    virtual ~IntentService();
    
    // Override this instead of onStartCommand
    virtual void onHandleIntent(Intent* intent) = 0;
    
    // IntentService handles onStartCommand internally
    int onStartCommand(Intent* intent, int flags, int startId) override;
};

/**
 * Global service instance registry.
 * Allows the JNI bridge to find service instances by class name.
 */
class ServiceRegistry {
public:
    using Factory = std::function<Service*()>;
    
    static void registerService(const std::string& name, Factory factory);
    static Service* createService(const std::string& name);
    static Service* getCurrentService(const std::string& name);
    static void setCurrentService(const std::string& name, Service* service);
    static void removeService(const std::string& name);
    
private:
    // Multiple services can run simultaneously, unlike activities
};

} // namespace aero

// ============================================================
// JNI Bridge Macros
// ============================================================

/**
 * AERO_REGISTER_SERVICE - Register service with the runtime.
 * Called at the end of service class definition.
 */
#define AERO_REGISTER_SERVICE(ClassName) \
    namespace { \
        struct ClassName##ServiceRegistrar { \
            ClassName##ServiceRegistrar() { \
                aero::ServiceRegistry::registerService(#ClassName, \
                    []() -> aero::Service* { return new ClassName(); }); \
            } \
        }; \
        static ClassName##ServiceRegistrar ClassName##_service_registrar; \
    }

// ============================================================
// JNI Native Methods (called from Java)
// ============================================================

extern "C" {

// Native service lifecycle callbacks and bridge methods omitted.
}
