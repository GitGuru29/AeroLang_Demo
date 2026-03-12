#pragma once

#include <cmath>

namespace aero {

/**
 * AeroLang Math utility class.
 * Provides standard mathematical functions and constants.
 */
class Math {
public:
    static constexpr double PI = 3.14159265358979323846;
    static constexpr double E = 2.71828182845904523536;

    static double abs(double value);
    static int abs(int value);
    
    static double pow(double base, double exponent);
    static double sqrt(double value);
    
    // Trigonometry
    static double sin(double value);
    static double cos(double value);
    static double tan(double value);
    static double asin(double value);
    static double acos(double value);
    static double atan(double value);
    static double atan2(double y, double x);

    // Min/Max/Clamp
    static double min(double a, double b);
    static int min(int a, int b);
    
    static double max(double a, double b);
    static int max(int a, int b);
    
    static double clamp(double value, double minVal, double maxVal);
    
    // Rounding
    static double round(double value);
    static double floor(double value);
    static double ceil(double value);
    
    // Logs
    static double log(double value);
    static double log10(double value);

    // Random
    static double random(); // Returns random number between [0.0, 1.0)
};

} // namespace aero
