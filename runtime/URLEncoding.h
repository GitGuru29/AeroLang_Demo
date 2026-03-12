#pragma once

#include <string>

namespace aero {

/**
 * AeroLang URL Encoding Utility.
 * Performs percent-encoding of URI components.
 */
class URLEncoding {
public:
    static std::string encode(const std::string& value);
    static std::string decode(const std::string& value);
};

} // namespace aero
