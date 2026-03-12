#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace aero {

/**
 * AeroLang Base64 Utility.
 * Encodes and decodes data in Base64 format.
 */
class Base64 {
public:
    static std::string encode(const std::vector<uint8_t>& data);
    static std::string encode(const std::string& text);
    
    static std::vector<uint8_t> decode(const std::string& base64Text);
    static std::string decodeToString(const std::string& base64Text);
    
private:
    static const char chars[];
    static const int decodeMap[];
};

} // namespace aero
