#pragma once

#include <functional>
#include <thread>
#include <atomic>
#include <map>
#include <mutex>

namespace aero {

/**
 * AeroLang Timer Utility.
 * Provides delayed and recurring executions mimicking setTimeout/setInterval.
 */
class Timer {
public:
    // Returns a unique timer ID
    static int setTimeout(std::function<void()> callback, int delayMs);
    static int setInterval(std::function<void()> callback, int intervalMs);
    
    static void clearTimeout(int timerId);
    static void clearInterval(int timerId);
    
    // Cleanup all timers (e.g., on application exit)
    static void clearAll();

private:
    struct TimerData {
        std::atomic<bool> active{true};
        std::thread thread;
    };
    
    static std::map<int, std::unique_ptr<TimerData>> timers;
    static std::mutex timersMutex;
    static int nextTimerId;
};

} // namespace aero
