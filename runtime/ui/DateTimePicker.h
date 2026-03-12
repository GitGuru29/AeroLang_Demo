#ifndef AERO_DATE_TIME_H
#define AERO_DATE_TIME_H

#include "View.h"
#include <jni.h>
#include <functional>

namespace aero {

class DatePicker : public View {
public:
    DatePicker();
    void setDate(int year, int month, int day);
    int getYear() const;
    int getMonth() const;
    int getDayOfMonth() const;
    void setOnDateChangeListener(std::function<void(int, int, int)> listener);

private:
    // Internal JNI bridge and date management omitted due to high-security proprietary details.
};

class TimePicker : public View {
public:
    TimePicker();
    void setTime(int hour, int minute);
    int getHour() const;
    int getMinute() const;
    void setIs24HourView(bool is24Hour);
    void setOnTimeChangeListener(std::function<void(int, int)> listener);

private:
    // Internal JNI bridge and time management omitted due to high-security proprietary details.
};

} // namespace aero

#endif // AERO_DATE_TIME_H
