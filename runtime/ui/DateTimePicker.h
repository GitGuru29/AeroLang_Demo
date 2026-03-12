#ifndef AERO_DATE_TIME_H
#define AERO_DATE_TIME_H

#include "View.h"
#include <jni.h>
#include <functional>

namespace aero {

class DatePicker : public View {
private:
    std::function<void(int, int, int)> dateChangeListener;

public:
    DatePicker() : dateChangeListener(nullptr) {
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass pickerClass = env->FindClass("android/widget/DatePicker");
        jmethodID constructor = env->GetMethodID(pickerClass, "<init>", "(Landroid/content/Context;)V");
        
        jobject context = getAndroidContext();
        viewObject = env->NewObject(pickerClass, constructor, context);
        
        env->DeleteLocalRef(pickerClass);
    }

    void setDate(int year, int month, int day) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass pickerClass = env->GetObjectClass(viewObject);
        jmethodID updateDateMethod = env->GetMethodID(pickerClass, "updateDate", "(III)V");
        
        env->CallVoidMethod(viewObject, updateDateMethod, year, month, day);
        env->DeleteLocalRef(pickerClass);
    }

    int getYear() const {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return 0;

        jclass pickerClass = env->GetObjectClass(viewObject);
        jmethodID getYearMethod = env->GetMethodID(pickerClass, "getYear", "()I");
        
        jint year = env->CallIntMethod(viewObject, getYearMethod);
        env->DeleteLocalRef(pickerClass);
        
        return static_cast<int>(year);
    }

    int getMonth() const {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return 0;

        jclass pickerClass = env->GetObjectClass(viewObject);
        jmethodID getMonthMethod = env->GetMethodID(pickerClass, "getMonth", "()I");
        
        jint month = env->CallIntMethod(viewObject, getMonthMethod);
        env->DeleteLocalRef(pickerClass);
        
        return static_cast<int>(month);
    }

    int getDayOfMonth() const {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return 0;

        jclass pickerClass = env->GetObjectClass(viewObject);
        jmethodID getDayMethod = env->GetMethodID(pickerClass, "getDayOfMonth", "()I");
        
        jint day = env->CallIntMethod(viewObject, getDayMethod);
        env->DeleteLocalRef(pickerClass);
        
        return static_cast<int>(day);
    }

    void setOnDateChangeListener(std::function<void(int, int, int)> listener) {
        dateChangeListener = listener;
    }
};

class TimePicker : public View {
private:
    std::function<void(int, int)> timeChangeListener;

public:
    TimePicker() : timeChangeListener(nullptr) {
        JNIEnv* env = getJNIEnv();
        if (!env) return;

        jclass pickerClass = env->FindClass("android/widget/TimePicker");
        jmethodID constructor = env->GetMethodID(pickerClass, "<init>", "(Landroid/content/Context;)V");
        
        jobject context = getAndroidContext();
        viewObject = env->NewObject(pickerClass, constructor, context);
        
        env->DeleteLocalRef(pickerClass);
    }

    void setTime(int hour, int minute) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass pickerClass = env->GetObjectClass(viewObject);
        
        jmethodID setHourMethod = env->GetMethodID(pickerClass, "setHour", "(I)V");
        jmethodID setMinuteMethod = env->GetMethodID(pickerClass, "setMinute", "(I)V");
        
        env->CallVoidMethod(viewObject, setHourMethod, hour);
        env->CallVoidMethod(viewObject, setMinuteMethod, minute);
        
        env->DeleteLocalRef(pickerClass);
    }

    int getHour() const {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return 0;

        jclass pickerClass = env->GetObjectClass(viewObject);
        jmethodID getHourMethod = env->GetMethodID(pickerClass, "getHour", "()I");
        
        jint hour = env->CallIntMethod(viewObject, getHourMethod);
        env->DeleteLocalRef(pickerClass);
        
        return static_cast<int>(hour);
    }

    int getMinute() const {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return 0;

        jclass pickerClass = env->GetObjectClass(viewObject);
        jmethodID getMinuteMethod = env->GetMethodID(pickerClass, "getMinute", "()I");
        
        jint minute = env->CallIntMethod(viewObject, getMinuteMethod);
        env->DeleteLocalRef(pickerClass);
        
        return static_cast<int>(minute);
    }

    void setIs24HourView(bool is24Hour) {
        JNIEnv* env = getJNIEnv();
        if (!env || !viewObject) return;

        jclass pickerClass = env->GetObjectClass(viewObject);
        jmethodID setIs24HourViewMethod = env->GetMethodID(pickerClass, "setIs24HourView", "(Ljava/lang/Boolean;)V");
        
        jclass booleanClass = env->FindClass("java/lang/Boolean");
        jmethodID valueOfMethod = env->GetStaticMethodID(booleanClass, "valueOf", "(Z)Ljava/lang/Boolean;");
        jobject booleanObj = env->CallStaticObjectMethod(booleanClass, valueOfMethod, is24Hour ? JNI_TRUE : JNI_FALSE);
        
        env->CallVoidMethod(viewObject, setIs24HourViewMethod, booleanObj);
        
        env->DeleteLocalRef(booleanObj);
        env->DeleteLocalRef(booleanClass);
        env->DeleteLocalRef(pickerClass);
    }

    void setOnTimeChangeListener(std::function<void(int, int)> listener) {
        timeChangeListener = listener;
    }
};

} // namespace aero

#endif // AERO_DATE_TIME_H
