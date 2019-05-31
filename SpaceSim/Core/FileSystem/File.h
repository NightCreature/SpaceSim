#pragma once

#include "Core/Types/Types.h"

namespace VFS
{


 //This should be an interface, with platform specific implementations of these functions like a mount point
class File
{
public:
    File() {}
    virtual ~File() {}

    virtual void Write(byte* data, size_t length) = 0;
    virtual byte* Read() = 0;
    virtual byte* Read(size_t amount) = 0;
};

}