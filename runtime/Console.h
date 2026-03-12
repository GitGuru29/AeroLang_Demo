#pragma once

#include <string>
#include <map>
#include <chrono>

namespace aero {

/**
 * AeroLang Console Utility.
 * High-level logging mimicking JS Console but routed through Log.
 */
class Console {
public:
    static void log(const std::string& msg);
    static void info(const std::string& msg);
    static void warn(const std::string& msg);
    static void error(const std::string& msg);
    
    // Assert
    static void assertCondition(bool condition, const std::string& msg);
    
    // Timing
    static void time(const std::string& label);
    static void timeEnd(const std::string& label);

private:
    static std::map<std::string, std::chrono::time_point<std::chrono::high_resolution_clock>> timers;
};

} // namespace aero
