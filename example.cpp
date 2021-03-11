#include <iostream>
#include <fstream>
#include <iomanip>
#include <ios>

#include "mem_file.h"

int createFile(const char *fileName,size_t size)
{
    size_t lines=size/11;
    if (lines>10)
        lines-=2;

   std::ofstream myFile("testfile.txt",std::ios::binary);

    size=size-lines*11;    
    for (size_t i=0;i<(size-1);++i)
        myFile << '#';
    myFile << std::endl;
    
    uint64_t inc=0x12345673;
    uint64_t val=inc;
    for (size_t i=0;i<lines;++i) {
        myFile << "0x" << std::hex << std::setw(8) << std::setfill('0') << (uint32_t)(val&0xffffffff) << std::endl;
        val+=inc;
    }
    
    // Close the file
    myFile.close();
    return 0;
}

#include "base64.h"

int main(int argc,char *argv[])
{
    const std::string fileName="testFile.txt";

    std::cout << "ZLIB stuff : MemFile().Encrypt().ErrorCode()=" << MemFile().Encrypt().ErrorCode() << "\n";

    createFile(fileName.c_str(),1024);    

    MemFile memFile;
    memFile.Read(fileName);

    std::cout << "Read file, size=" << memFile.Size() << ", errorCode=" << memFile.ErrorCode() << "\n";

    memFile.Encrypt();

    std::cout << "Encrypted file, size=" << memFile.Size() << ", errorCode=" << memFile.ErrorCode() << "\n";

    std::string b64=base64_encode((const unsigned char*)memFile.Data(),memFile.Size());
    std::cout << "Base64 size=" << b64.size() << "\n";
    std::cout << "Base64 data=\n\"" << b64 << "\"\n";
    
    std::cout << "<MemFile FileName=\"" << memFile.FileName() << "\" Format=\"3\">" << b64 << "</MemFile>\n";

    std::vector<char> crypt=base64_decode(b64);
    std::cout << "Encrypted data from b64, size=" << crypt.size() << "\n";

    {
        MemFile newMemFile("resultFile.txt");
        newMemFile.Assign(crypt.data(), crypt.size(), true);
        newMemFile.Save("resultFile.b64");

        std::cout << "New: FileName=\"" << newMemFile.FileName() << "\", size=" << newMemFile.Size() << ", encrypted=" << (newMemFile.Encrypted() ? "Yes" : "No") << "\n";
        newMemFile.Decrypt();
        std::cout << "New: FileName=\"" << newMemFile.FileName() << "\", size=" << newMemFile.Size() << ", encrypted=" << (newMemFile.Encrypted() ? "Yes" : "No") << "\n";
        newMemFile.Save();
    }

    {
        MemFile newMemFile("resultFile2.txt");
        newMemFile.Read("resultFile.b64");

        std::cout << "B64 file: FileName=\"" << newMemFile.FileName() << "\", size=" << newMemFile.Size() << ", encrypted=" << (newMemFile.Encrypted() ? "Yes" : "No") << "\n";
        newMemFile.Decrypt();
        std::cout << "B64 file: FileName=\"" << newMemFile.FileName() << "\", size=" << newMemFile.Size() << ", encrypted=" << (newMemFile.Encrypted() ? "Yes" : "No") << "\n";

        newMemFile.Save();
    }
    return 0;
}