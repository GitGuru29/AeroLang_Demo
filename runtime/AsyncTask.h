#pragma once

#include <jni.h>
#include <functional>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include <memory>
#include <string>

namespace aero {

/**
 * AsyncTask - Run background work and deliver results on the UI thread.
 * 
 * Usage:
 *   class DownloadTask : public AsyncTask<std::string> {
 *       std::string doInBackground() override {
 *           // Heavy work on background thread
 *           return "result";
 *       }
 *       void onPostExecute(const std::string& result) override {
 *           // Update UI on main thread
 *       }
 *   };
 *   
 *   DownloadTask task;
 *   task.execute();
 */
template<typename Result>
class AsyncTask {
public:
    AsyncTask() : cancelled(false), env(nullptr) {}
    virtual ~AsyncTask() {}
    
    // Override these in subclass
    virtual void onPreExecute() {}
    virtual Result doInBackground() = 0;
    virtual void onPostExecute(const Result& result) {}
    virtual void onProgressUpdate(int progress) {}
    virtual void onCancelled() {}
    
    // Execute the task
    void execute() {
        onPreExecute();
        
        std::thread worker([this]() {
            if (!cancelled) {
                Result result = doInBackground();
                
                if (!cancelled) {
                    // Post result to UI thread
                    postToUiThread([this, result]() {
                        onPostExecute(result);
                    });
                } else {
                    postToUiThread([this]() {
                        onCancelled();
                    });
                }
            }
        });
        worker.detach();
    }
    
    // Cancel the task
    void cancel() {
        cancelled = true;
    }
    
    bool isCancelled() const {
        return cancelled;
    }
    
    // Set JNI env for UI thread posting
    void initJNI(JNIEnv* env) {
        this->env = env;
    }
    
protected:
    // Report progress from background thread
    void publishProgress(int progress) {
        postToUiThread([this, progress]() {
            onProgressUpdate(progress);
        });
    }
    
private:
    std::atomic<bool> cancelled;
    JNIEnv* env;
    
    void postToUiThread(std::function<void()> callback) {
        if (!env) {
            // If no JNI env, execute directly (for testing)
            callback();
            return;
        }
        
        // Post to Android main thread via Handler
        jclass looperClass = env->FindClass("android/os/Looper");
        if (!looperClass) {
            callback();
            return;
        }
        
        jmethodID getMainLooper = env->GetStaticMethodID(looperClass, "getMainLooper",
            "()Landroid/os/Looper;");
        jobject mainLooper = env->CallStaticObjectMethod(looperClass, getMainLooper);
        
        jclass handlerClass = env->FindClass("android/os/Handler");
        jmethodID handlerConstructor = env->GetMethodID(handlerClass, "<init>",
            "(Landroid/os/Looper;)V");
        jobject handler = env->NewObject(handlerClass, handlerConstructor, mainLooper);
        
        // Store callback for JNI access
        auto* callbackPtr = new std::function<void()>(callback);
        
        jclass runnableClass = env->FindClass("com/aerolang/runtime/AeroRunnable");
        if (runnableClass) {
            jmethodID runnableConstructor = env->GetMethodID(runnableClass, "<init>", "(J)V");
            jobject runnable = env->NewObject(runnableClass, runnableConstructor, (jlong)callbackPtr);
            
            jmethodID post = env->GetMethodID(handlerClass, "post",
                "(Ljava/lang/Runnable;)Z");
            env->CallBooleanMethod(handler, post, runnable);
            
            env->DeleteLocalRef(runnable);
            env->DeleteLocalRef(runnableClass);
        } else {
            // Fallback: execute directly
            callback();
            delete callbackPtr;
        }
        
        env->DeleteLocalRef(handler);
        env->DeleteLocalRef(handlerClass);
        env->DeleteLocalRef(mainLooper);
        env->DeleteLocalRef(looperClass);
    }
};

/**
 * Convenience function: run a lambda on a background thread.
 */
inline void runAsync(std::function<void()> task) {
    std::thread worker(task);
    worker.detach();
}

/**
 * Convenience function: run a lambda on the UI thread.
 * Requires JNI environment.
 */
void runOnUiThread(JNIEnv* env, std::function<void()> callback);

/**
 * Convenience function: delay execution.
 */
inline void delay(long milliseconds, std::function<void()> callback) {
    std::thread worker([milliseconds, callback]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
        callback();
    });
    worker.detach();
}

/**
 * Timer - Repeating interval execution.
 * 
 * Usage:
 *   Timer timer;
 *   timer.setInterval(1000); // 1 second
 *   timer.setCallback([]() {
 *       // Called every second
 *   });
 *   timer.start();
 *   // ... later
 *   timer.stop();
 */
class Timer {
public:
    Timer() : intervalMs(1000), running(false) {}
    ~Timer() { stop(); }
    
    Timer& setInterval(long ms) { intervalMs = ms; return *this; }
    Timer& setCallback(std::function<void()> cb) { callback = cb; return *this; }
    
    void start() {
        if (running) return;
        running = true;
        
        std::thread worker([this]() {
            while (running) {
                std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
                if (running && callback) {
                    callback();
                }
            }
        });
        worker.detach();
    }
    
    void stop() {
        running = false;
    }
    
    bool isRunning() const {
        return running;
    }
    
private:
    long intervalMs;
    std::atomic<bool> running;
    std::function<void()> callback;
};

/**
 * Handler - Post messages and runnables to a thread's message queue.
 * Wraps Android's Handler via JNI.
 */
class Handler {
public:
    Handler();
    Handler(JNIEnv* env);
    ~Handler();
    
    void initJNI(JNIEnv* env);
    
    // Post a callback to execute on the handler's thread
    void post(std::function<void()> callback);
    void postDelayed(std::function<void()> callback, long delayMs);
    
    // Remove pending callbacks
    void removeCallbacks();
    
private:
    JNIEnv* env;
    jobject handlerInstance;
};

} // namespace aero

// ============================================================
// JNI Entry Point for AeroRunnable callbacks
// ============================================================

extern "C" {

JNIEXPORT void JNICALL
Java_com_aerolang_runtime_AeroRunnable_nativeRun(JNIEnv* env, jobject thiz, jlong callbackPtr);

}
