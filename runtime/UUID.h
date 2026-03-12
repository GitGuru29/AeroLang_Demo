#pragma once

#include <string>

namespace aero {

/**
 * AeroLang UUID Utility.
 * Generates universally unique identifiers (v4).
 */
class UUID {
public:
    static std::string v4();
};

} // namespace aero
