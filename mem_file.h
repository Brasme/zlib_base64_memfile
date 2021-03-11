#pragma once
#include <string>
#include <memory>

class MemFile {
public:
    MemFile();
    explicit MemFile(const std::string &fileName);
    typedef uint8_t format_t;

    bool Encrypted() const;
    MemFile& Encrypt();
    MemFile& Decrypt();
    MemFile& Read(const std::string& fileName);
    MemFile& Save();
    MemFile& Save(const std::string& fileName);
    MemFile& Assign(const char *data,size_t size,bool encrypted);
    size_t Size() const;
    const char* Data() const;
    const std::string &FileName() const;
    int ErrorCode() const;    
private:
    class Impl;
    std::shared_ptr<Impl> impl_;
};