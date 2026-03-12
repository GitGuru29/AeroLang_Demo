#pragma once

#include <string>
#include <ctime>
#include <chrono>

namespace aero {

/**
 * AeroLang DateTime utility class.
 * Handles dates, times, formatting, and timestamps.
 */
class DateTime {
public:
    DateTime(); // Current time
    explicit DateTime(int64_t timestampMillis);
    DateTime(int year, int month, int day, int hour = 0, int minute = 0, int second = 0);

    // Getters
    int getYear() const;
    int getMonth() const; // 1-12
    int getDay() const;   // 1-31
    int getHour() const;  // 0-23
    int getMinute() const; // 0-59
    int getSecond() const; // 0-59
    int getDayOfWeek() const; // 0 (Sun) - 6 (Sat)
    
    int64_t toTimestampMillis() const;

    // Static utilities
    static int64_t now();
    static DateTime parse(const std::string& dateString, const std::string& format);

    // Format
    std::string format(const std::string& formatStr) const;
    std::string toISOString() const;

private:
    std::chrono::system_clock::time_point timePoint;
    std::tm toTm() const;
};

} // namespace aero
