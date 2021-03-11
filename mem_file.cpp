#include "mem_file.h"

#include <iostream>
#include <zlib.h>
#include <fstream>
#include <iomanip>
#include <ios>
#include <vector>
#include "base64.h"

class MemFile::Impl
{
public:
    Impl(): errorCode_(0), fileName_("") {}
    explicit Impl(const std::string &fileName): errorCode_(0), fileName_(fileName) {}
    ~Impl() {
        Clear();
    }
    void Clear() {
        fileBuffer_.clear();
        encryptBuffer_.clear();
    }

    void Encrypt() {
        if (encryptBuffer_.size()>0)
            return;
        if (fileBuffer_.size()==0) {
            errorCode_=-5;
            return;
        }
        uLongf dstSize=compressBound(fileBuffer_.size());
        encryptBuffer_.resize(dstSize);
        Bytef *dest=(Bytef*)encryptBuffer_.data();
        if (compress(dest,&dstSize,(Bytef*)fileBuffer_.data(),fileBuffer_.size())!=Z_OK) {
            errorCode_=-6;
            encryptBuffer_.resize(0);
            return;
        }
        encryptBuffer_.resize(dstSize);
        fileBuffer_.clear();
        errorCode_=0;
    }

    void Decrypt() {
        if (fileBuffer_.size()>0)
            return;
        if (encryptBuffer_.size()==0) {
            errorCode_=-8;
            return;
        }

        for (size_t scale=4;scale<=16;scale*=2) {
            uLongf dstSize=encryptBuffer_.size()*scale;
            fileBuffer_.resize(dstSize);
            Bytef *dest=(Bytef*)fileBuffer_.data();
            if (uncompress(dest,&dstSize,(Bytef*)encryptBuffer_.data(),encryptBuffer_.size())==Z_OK) {
                errorCode_=0;
                fileBuffer_.resize(dstSize);
                encryptBuffer_.resize(0);
                return;
            }
        }
        fileBuffer_.clear();
        errorCode_=-9;        
    }

    void Read(const std::string& fileName) {
        Clear();
        std::ifstream file(fileName.c_str(), std::ios::binary | std::ios::ate);
        std::streamsize size = file.tellg();
        if (size<=0) {
            errorCode_=-2;
            return;
        }
        if (size>0x1000000) {
            errorCode_=-3;
            return;
        }
        
        file.seekg(0, std::ios::beg);
        
        char header[6];
        char *dst=header;
        if (size>=4) {
            if (file.read(dst,4)) {
                if (memcmp(dst,"B64:",4)==0) {
                    std::string line;
                    std::getline(file,line); // B64 line
                    std::getline(file,line); // B64 encoded
                    encryptBuffer_=base64_decode(line);
                    errorCode_=0;
                    return;
                }
            } else {
                errorCode_=-15;
                return;
            }
            fileBuffer_.resize((size_t)size);
            dst=fileBuffer_.data();
            memcpy(dst,header,4);
            dst+=4;
            size-=4;
        } else {
            fileBuffer_.resize((size_t)size);
            dst=fileBuffer_.data();            
        }

        if (file.read(dst, size))
        { // ok
            errorCode_=0;
            if (fileName_.empty())
                fileName_=fileName;
            return;
        }   
        errorCode_=-4;   
        Clear();              
    }
    void Save() {
        return Save(fileName_);
    }

    void Save(const std::string& fileName) {
        if (fileName.empty()) {
            errorCode_=-11;
            return;
        }

        std::ofstream file(fileName.c_str(), std::ios::binary);
        if (fileBuffer_.size()>0) {
            if(file.write(fileBuffer_.data(),fileBuffer_.size())) {
                errorCode_=0;
                return;
            }
            errorCode_=-12;
            return;
        }

        if (encryptBuffer_.size()>0) {
            file << "B64:\n";
            std::string b64=base64_encode((const unsigned char*)encryptBuffer_.data(),encryptBuffer_.size());
            file << b64;
            errorCode_=0;
            return;
        }
        errorCode_=-13;
    }    
    size_t Size() const { 
        size_t size = fileBuffer_.size(); 
        if (size==0) size=encryptBuffer_.size();
        return size;
    }
    bool Encrypted() const {
        return encryptBuffer_.size()>0;        
    }
    const char *Data() const {
        if (fileBuffer_.size()>0)
            return fileBuffer_.data();
        if (encryptBuffer_.size()>0)
            return encryptBuffer_.data();
        static const char empty='\0';
        return &empty;
    }
    void Assign(const char *data,size_t size,bool encrypted)
    {
        Clear();
        char *dst=nullptr;
        if (encrypted) {
            encryptBuffer_.resize(size);
            dst=encryptBuffer_.data();
        } else {
            fileBuffer_.resize(size);
            dst=fileBuffer_.data();
        }
        memcpy(dst,data,size);
        errorCode_=0;
    }
    const std::string &FileName() const { return fileName_; }
    int ErrorCode() const { return errorCode_;}
private:
    std::vector<char> fileBuffer_;
    std::vector<char> encryptBuffer_;
    std::string fileName_;
    int errorCode_;
};

MemFile::MemFile(): impl_(new Impl())
{
} 

MemFile::MemFile(const std::string &fileName): impl_(new Impl(fileName))
{
} 

int MemFile::ErrorCode() const 
{
    return impl_->ErrorCode();
}

const std::string &MemFile::FileName() const 
{
    return impl_->FileName();
}

size_t MemFile::Size() const 
{
    return impl_->Size();
}

MemFile& MemFile::Read(const std::string& fileName)
{
    impl_->Read(fileName);
    return *this;
}

MemFile& MemFile::Save(const std::string& fileName)
{
    impl_->Save(fileName);
    return *this;
}

MemFile& MemFile::Save()
{
    impl_->Save();
    return *this;
}

MemFile& MemFile::Assign(const char *data,size_t size, bool encrypted)
{
    impl_->Assign(data,size,encrypted);
    return *this;
}

MemFile& MemFile::Encrypt()
{
    impl_->Encrypt();    
    return *this;
} 

MemFile& MemFile::Decrypt()
{
    impl_->Decrypt();    
    return *this;
} 

const char* MemFile::Data() const
{
    return impl_->Data();
}

bool MemFile::Encrypted() const
{
    return impl_->Encrypted();
}
