#pragma once

#include <jni.h>
#include <string>
#include <vector>
#include <functional>
#include <memory>

namespace aero {

/**
 * Work result returned by Worker::doWork().
 */
class WorkResult {
public:
    enum class Status { SUCCESS, FAILURE, RETRY };
    
    static WorkResult success() { return WorkResult(Status::SUCCESS); }
    static WorkResult failure() { return WorkResult(Status::FAILURE); }
    static WorkResult retry() { return WorkResult(Status::RETRY); }
    
    Status getStatus() const { return status; }
    bool isSuccess() const { return status == Status::SUCCESS; }
    
private:
    Status status;
    explicit WorkResult(Status s) : status(s) {}
};

/**
 * Constraints for when work should run.
 * Builder pattern for easy configuration.
 */
class Constraints {
public:
    Constraints() 
        : networkRequired(false), chargingRequired(false),
          idleRequired(false), storageNotLowRequired(false),
          batteryNotLowRequired(false) {}
    
    Constraints& setRequiresNetwork(bool required) { networkRequired = required; return *this; }
    Constraints& setRequiresCharging(bool required) { chargingRequired = required; return *this; }
    Constraints& setRequiresDeviceIdle(bool required) { idleRequired = required; return *this; }
    Constraints& setRequiresStorageNotLow(bool required) { storageNotLowRequired = required; return *this; }
    Constraints& setRequiresBatteryNotLow(bool required) { batteryNotLowRequired = required; return *this; }
    
    bool requiresNetwork() const { return networkRequired; }
    bool requiresCharging() const { return chargingRequired; }
    bool requiresDeviceIdle() const { return idleRequired; }
    bool requiresStorageNotLow() const { return storageNotLowRequired; }
    bool requiresBatteryNotLow() const { return batteryNotLowRequired; }
    
private:
    bool networkRequired;
    bool chargingRequired;
    bool idleRequired;
    bool storageNotLowRequired;
    bool batteryNotLowRequired;
};

/**
 * Worker - Base class for background work units.
 * Override doWork() to perform the background task.
 */
class Worker {
public:
    Worker() : env(nullptr) {}
    virtual ~Worker() {}
    
    virtual WorkResult doWork() = 0;
    
    // Access input data
    std::string getInputString(const std::string& key, const std::string& defaultValue = "") const;
    int getInputInt(const std::string& key, int defaultValue = 0) const;
    bool getInputBool(const std::string& key, bool defaultValue = false) const;
    
    void initJNI(JNIEnv* env) { this->env = env; }
    
protected:
    JNIEnv* env;
};

/**
 * WorkRequest - Describes a unit of work to be scheduled.
 */
class WorkRequest {
public:
    enum class Type { ONE_TIME, PERIODIC };
    
    // Builder pattern
    WorkRequest& setTag(const std::string& tag) { this->tag = tag; return *this; }
    WorkRequest& setConstraints(const Constraints& constraints) { this->constraints = constraints; return *this; }
    WorkRequest& setInitialDelay(long delayMs) { this->initialDelayMs = delayMs; return *this; }
    WorkRequest& addInputString(const std::string& key, const std::string& value);
    WorkRequest& addInputInt(const std::string& key, int value);
    WorkRequest& addInputBool(const std::string& key, bool value);
    WorkRequest& setBackoffPolicy(long backoffDelayMs) { this->backoffDelayMs = backoffDelayMs; return *this; }
    
    // Getters
    Type getType() const { return type; }
    const std::string& getTag() const { return tag; }
    const Constraints& getConstraints() const { return constraints; }
    long getInitialDelay() const { return initialDelayMs; }
    long getRepeatInterval() const { return repeatIntervalMs; }
    const std::string& getWorkerName() const { return workerName; }
    
    // Build the JNI WorkRequest object
    jobject buildJNI(JNIEnv* env) const;
    
protected:
    Type type;
    std::string workerName;
    std::string tag;
    Constraints constraints;
    long initialDelayMs = 0;
    long repeatIntervalMs = 0;
    long backoffDelayMs = 30000; // 30 second default
    
    friend class OneTimeWorkRequest;
    friend class PeriodicWorkRequest;
};

/**
 * OneTimeWorkRequest - Runs work exactly once.
 */
class OneTimeWorkRequest : public WorkRequest {
public:
    explicit OneTimeWorkRequest(const std::string& workerClassName) {
        type = Type::ONE_TIME;
        workerName = workerClassName;
    }
};

/**
 * PeriodicWorkRequest - Runs work at regular intervals.
 * Minimum interval is 15 minutes (Android constraint).
 */
class PeriodicWorkRequest : public WorkRequest {
public:
    PeriodicWorkRequest(const std::string& workerClassName, long intervalMs) {
        type = Type::PERIODIC;
        workerName = workerClassName;
        repeatIntervalMs = intervalMs;
    }
    
    // Minimum 15 minutes
    static constexpr long MIN_PERIODIC_INTERVAL_MS = 15 * 60 * 1000;
};

/**
 * WorkManager - Schedule and manage background work.
 * Wraps Android's WorkManager via JNI.
 */
class WorkManager {
public:
    // Get singleton instance
    static WorkManager& getInstance();
    
    // Initialize with JNI environment
    void initJNI(JNIEnv* env, jobject context);
    
    // Enqueue work
    void enqueue(const WorkRequest& request);
    void enqueueUnique(const std::string& uniqueName, const WorkRequest& request);
    
    // Cancel work
    void cancelByTag(const std::string& tag);
    void cancelUnique(const std::string& uniqueName);
    void cancelAll();
    
    // Chaining (for sequential work)
    WorkManager& beginWith(const WorkRequest& request);
    WorkManager& then(const WorkRequest& request);
    void enqueueChain();
    
    // Existing unique work policies
    enum class ExistingWorkPolicy { REPLACE, KEEP, APPEND };
    void enqueueUnique(const std::string& uniqueName, ExistingWorkPolicy policy, 
                       const WorkRequest& request);
    
private:
    WorkManager();
    ~WorkManager();
    
    JNIEnv* env;
    jobject workManagerInstance;
    std::vector<WorkRequest> chainedWork;
};

/**
 * Worker Registry - maps worker names to factories.
 */
class WorkerRegistry {
public:
    using Factory = std::function<Worker*()>;
    
    static void registerWorker(const std::string& name, Factory factory);
    static Worker* createWorker(const std::string& name);
};

#define AERO_REGISTER_WORKER(ClassName) \
    namespace { \
        struct ClassName##WorkerRegistrar { \
            ClassName##WorkerRegistrar() { \
                aero::WorkerRegistry::registerWorker(#ClassName, \
                    []() -> aero::Worker* { return new ClassName(); }); \
            } \
        }; \
        static ClassName##WorkerRegistrar ClassName##_worker_registrar; \
    }

} // namespace aero
