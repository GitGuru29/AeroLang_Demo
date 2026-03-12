#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <jni.h>

namespace aero {

// Forward declaration
class Activity;

/**
 * FileIO provides static methods for reading and writing files.
 * Works with Android's internal storage by default.
 * 
 * Usage:
 *   // Write text
 *   FileIO::writeText(activity, "config.txt", "Hello World");
 *   
 *   // Read text
 *   std::string content = FileIO::readText(activity, "config.txt");
 *   
 *   // Write binary
 *   std::vector<uint8_t> data = {0x00, 0x01, 0x02};
 *   FileIO::writeBytes(activity, "data.bin", data);
 */
class FileIO {
public:
    // Storage modes
    enum StorageMode {
        INTERNAL,    // App-private internal storage
        CACHE,       // Cache directory (may be cleared by system)
        EXTERNAL     // External storage (requires permissions)
    };
    
    // Text file operations
    static std::string readText(Activity* activity, const std::string& filename, 
                                 StorageMode mode = INTERNAL);
    static bool writeText(Activity* activity, const std::string& filename, 
                          const std::string& content, StorageMode mode = INTERNAL);
    static bool appendText(Activity* activity, const std::string& filename, 
                           const std::string& content, StorageMode mode = INTERNAL);
    
    // Binary file operations
    static std::vector<uint8_t> readBytes(Activity* activity, const std::string& filename,
                                           StorageMode mode = INTERNAL);
    static bool writeBytes(Activity* activity, const std::string& filename,
                           const std::vector<uint8_t>& data, StorageMode mode = INTERNAL);
    
    // File management
    static bool exists(Activity* activity, const std::string& filename, 
                       StorageMode mode = INTERNAL);
    static bool deleteFile(Activity* activity, const std::string& filename,
                           StorageMode mode = INTERNAL);
    static std::vector<std::string> listFiles(Activity* activity, 
                                               StorageMode mode = INTERNAL);
    static long fileSize(Activity* activity, const std::string& filename,
                         StorageMode mode = INTERNAL);
    
    // Directory operations
    static bool createDirectory(Activity* activity, const std::string& dirname,
                                StorageMode mode = INTERNAL);
    static bool deleteDirectory(Activity* activity, const std::string& dirname,
                                StorageMode mode = INTERNAL);
    
    // Get storage paths
    static std::string getInternalPath(Activity* activity);
    static std::string getCachePath(Activity* activity);
    static std::string getExternalPath(Activity* activity);
    
private:
    static std::string getBasePath(Activity* activity, StorageMode mode);
    static std::string getFullPath(Activity* activity, const std::string& filename, 
                                   StorageMode mode);
};

} // namespace aero
