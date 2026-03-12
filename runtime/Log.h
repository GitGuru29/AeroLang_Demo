#pragma once

#include <string>

namespace aero {

/**
 * AeroLang System Logger.
 * Interfaces with platform logs (Android Logcat).
 */
class Log {
public:
    static void d(const std::string& tag, const std::string& msg); // Debug
    static void i(const std::string& tag, const std::string& msg); // Info
    static void w(const std::string& tag, const std::string& msg); // Warning
    static void e(const std::string& tag, const std::string& msg); // Error
};

} // namespace aero
