#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <jni.h>

namespace aero {

/**
 * AeroLang Cryptography Utility.
 * Provides hashing and basic crypto standard wrappers via JNI to Android's MessageDigest.
 */
class Crypto {
public:
    static std::string md5(const std::string& input);
    static std::string sha256(const std::string& input);
    
    // Hash binary directly
    static std::string md5(const std::vector<uint8_t>& input);
    static std::string sha256(const std::vector<uint8_t>& input);

private:
    static std::string hash(const std::vector<uint8_t>& input, const char* algorithm);
    static std::string bytesToHex(const std::vector<uint8_t>& bytes);
};

} // namespace aero
