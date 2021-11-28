#pragma once

#include "FileSystem/File.h"

#include <fstream>

namespace VFS
{

//Common file use STL to do serialisation this makes it work on any platform when lacking platform specfics
class CommonFile :public File
{
public:

    void Write(byte* data, size_t length) override;
    byte* Read() override;
    byte* Read(size_t amount) override;
private:

    std::fstream m_actualFile;
};

}