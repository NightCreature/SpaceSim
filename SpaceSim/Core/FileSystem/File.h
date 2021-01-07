#pragma once

#include "Core/FileSystem/Flags.h"
#include "Core/Types/Types.h"

namespace VFS
{


 //This is the interface for a file all functions are implemented in platform specifics
class File
{
public:
    File() : m_platformSpecificData(nullptr) {}
    ~File() {}

    void Close();

    void Write(byte* data, size_t length);
    byte* Read(byte*& data);
    byte* Read(byte*& data, size_t amount);

    bool IsValid() const { return m_platformSpecificData != nullptr; }

    void createFile(const std::filesystem::path& name, FileMode fileMode);
private:
    std::filesystem::path m_name;
    void* m_platformSpecificData = nullptr; //Contains things like the file handle
};

}