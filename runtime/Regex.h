#pragma once

#include <string>
#include <vector>
#include <regex>

namespace aero {

/**
 * AeroLang Regex utility class.
 * Wrapper around std::regex for matching, searching, and replacing.
 */
class Regex {
public:
    explicit Regex(const std::string& pattern);
    
    bool match(const std::string& subject) const; // Full match
    bool search(const std::string& subject) const; // Substring match
    
    // Replace all instances
    std::string replace(const std::string& subject, const std::string& replacement) const;
    
    // Find all matches (returns matched strings)
    std::vector<std::string> findAll(const std::string& subject) const;

private:
    std::regex re;
};

} // namespace aero
