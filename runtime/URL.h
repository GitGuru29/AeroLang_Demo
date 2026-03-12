#pragma once

#include <string>
#include <map>

namespace aero {

/**
 * AeroLang URL Utility.
 * Parses and builds URLs and Query parameters.
 */
class URL {
public:
    URL();
    explicit URL(const std::string& urlString);
    
    std::string getScheme() const;
    std::string getHost() const;
    int getPort() const; // Returns -1 if not set
    std::string getPath() const;
    std::string getQuery() const;
    std::string getFragment() const;
    
    // Setters
    URL& setScheme(const std::string& scheme);
    URL& setHost(const std::string& host);
    URL& setPort(int port);
    URL& setPath(const std::string& path);
    URL& setQuery(const std::string& query);
    URL& setFragment(const std::string& fragment);
    
    // Query parameters
    std::map<std::string, std::string> getQueryParams() const;
    URL& addQueryParam(const std::string& key, const std::string& value);
    URL& removeQueryParam(const std::string& key);
    
    std::string toString() const;

private:
    std::string scheme;
    std::string host;
    int port;
    std::string path;
    std::map<std::string, std::string> queryParams;
    std::string fragment;
    
    void parseQuery(const std::string& queryStr);
    std::string buildQueryString() const;
};

} // namespace aero
