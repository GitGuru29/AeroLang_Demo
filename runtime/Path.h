#pragma once

#include <string>
#include <vector>

namespace aero {

/**
 * AeroLang Path Utility.
 * Provides abstraction for interacting with filesystem path strings.
 */
class Path {
public:
    static std::string join(const std::string& path1, const std::string& path2);
    static std::string join(const std::vector<std::string>& paths);
    
    static std::string getExtension(const std::string& path);
    static std::string getFilename(const std::string& path);
    static std::string getParent(const std::string& path);
    
    static std::string normalize(const std::string& path);
    static bool isAbsolute(const std::string& path);
    
    static const char SEPARATOR;
};

} // namespace aero
