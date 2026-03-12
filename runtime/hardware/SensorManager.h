#pragma once

#include <vector>
#include <jni.h>

namespace aero {

/**
 * aero::SensorManager
 * Accesses device sensors like Accelerometer and Gyroscope.
 */
class SensorManager {
public:
    SensorManager();
    ~SensorManager();

    static void initJNI(JNIEnv* env);

    struct SensorData {
        float x;
        float y;
        float z;
    };

    // Synchronous poll (Last recorded state)
    SensorData getAccelerometerData();
    SensorData getGyroscopeData();
    SensorData getCompassData();       // Magnetometer (XYZ micro-Tesla)

    float getProximityData();          // Distance in cm
    float getAmbientTemperature();     // Temperature in Celsius

    // Start/Stop listeners
    void startListening();
    void stopListening();

private:
    JNIEnv* env;
    SensorData accelData;
    SensorData gyroData;

    static JNIEnv* getDefaultEnv();
};

} // namespace aero
