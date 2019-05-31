#include "Core/FileSystem/Platform/Win/FileWin.h"

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void VFS::FileWin::Write(byte* data, size_t length)
{
    throw std::logic_error("The method or operation is not implemented.");
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
byte* VFS::FileWin::Read()
{
    throw std::logic_error("The method or operation is not implemented.");
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
byte* VFS::FileWin::Read(size_t amount)
{
    throw std::logic_error("The method or operation is not implemented.");
}

