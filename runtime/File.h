#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include "FileIO.h"

namespace aero {

class Activity;

/**
 * File object-oriented wrapper over FileIO.
 * Represents a file path and provides operations on it.
 */
class File {
public:
    File(Activity* activity, const std::string& path, FileIO::StorageMode mode = FileIO::INTERNAL);
    ~File();

    std::string getPath() const;
    FileIO::StorageMode getMode() const;
    
    // File operations
    bool exists() const;
    long size() const;
    bool deleteFile() const;
    
    // I/O operations
    std::string readText() const;
    bool writeText(const std::string& content) const;
    bool appendText(const std::string& content) const;
    
    std::vector<uint8_t> readBytes() const;
    bool writeBytes(const std::vector<uint8_t>& data) const;
    
    // Directory operations
    bool isDirectory() const; // For now relies on create/list
    bool createDirectory() const;
    bool deleteDirectory() const;
    std::vector<std::string> listFiles() const;

private:
    Activity* activity;
    std::string path;
    FileIO::StorageMode mode;
};

} // namespace aero
