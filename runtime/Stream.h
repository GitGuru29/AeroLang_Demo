#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <memory>

namespace aero {

/**
 * AeroLang InputStream Utility.
 * Reads data from a source sequentially.
 */
class InputStream {
public:
    virtual ~InputStream() = default;
    
    virtual int read() = 0; // Read single byte, returns -1 on EOF
    virtual size_t read(std::vector<uint8_t>& buffer, size_t offset, size_t length) = 0;
    virtual std::vector<uint8_t> readAll() = 0;
    
    virtual void close() = 0;
    virtual size_t available() const = 0;
};

/**
 * AeroLang OutputStream Utility.
 * Writes data to a destination sequentially.
 */
class OutputStream {
public:
    virtual ~OutputStream() = default;
    
    virtual void write(int b) = 0; // Write single byte
    virtual void write(const std::vector<uint8_t>& buffer, size_t offset, size_t length) = 0;
    
    virtual void flush() = 0;
    virtual void close() = 0;
};

// --- Implementations for Files ---

class FileInputStream : public InputStream {
public:
    explicit FileInputStream(const std::string& path);
    ~FileInputStream() override;
    
    int read() override;
    size_t read(std::vector<uint8_t>& buffer, size_t offset, size_t length) override;
    std::vector<uint8_t> readAll() override;
    
    void close() override;
    size_t available() const override;

private:
    std::ifstream file;
    size_t fileSize;
};

class FileOutputStream : public OutputStream {
public:
    FileOutputStream(const std::string& path, bool append = false);
    ~FileOutputStream() override;
    
    void write(int b) override;
    void write(const std::vector<uint8_t>& buffer, size_t offset, size_t length) override;
    
    void flush() override;
    void close() override;

private:
    std::ofstream file;
};

} // namespace aero
