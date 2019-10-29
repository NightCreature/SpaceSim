#pragma once

#include "Core/FileSystem/File.h"

namespace VFS
{

class FileWin : public File
{
public:

    FileWin() {}
    virtual ~FileWin() { CloseHandle(m_fileHandle); }

    virtual void Write(byte* data, size_t length) override;
    virtual byte* Read() override;
    virtual byte* Read(size_t amount) override;
private:
    HANDLE m_fileHandle;
};

}