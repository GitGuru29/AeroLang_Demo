#pragma once

#include <string>
#include <jni.h>

namespace aero {

/**
 * aero::LocationManager
 * Accesses device location hardware.
 * Note: Requires ACCESS_FINE_LOCATION in AndroidManifest.
 */
class LocationManager {
public:
    LocationManager();
    ~LocationManager();

    // Initializes JNI references
    static void initJNI(JNIEnv* env);

    // Synchronous MVP poll for last known coordinates
    double getLatitude();
    double getLongitude();

    // Requests location updates to be handled asynchronously (Stub for MVP)
    void requestLocationUpdates();
    void stopLocationUpdates();

private:
    jobject locationManagerObj;
    JNIEnv* env;
    
    double lastLat;
    double lastLon;

    static JNIEnv* getDefaultEnv();
};

} // namespace aero
