#include "Core/FileSystem/Platform/Win/MountPointWin.h"

#include "Core/FileSystem/File.h"
#include "Core/Types/TypeHelpers.h"

namespace VFS
{

///-----------------------------------------------------------------------------
///! @brief Represents a directory on the Windows filesystem
///! @remark 
///-----------------------------------------------------------------------------
MountPointWin::MountPointWin(const std::filesystem::path& path) : MountPoint(path)
{
    //make sure this path actually exists
    WIN32_FILE_ATTRIBUTE_DATA fileAttributeData;
    ZeroMemory(&fileAttributeData, sizeof(WIN32_FILE_ATTRIBUTE_DATA));
    if ( GetFileAttributesEx(path.string().c_str(), GetFileExInfoStandard, static_cast<void*>(&fileAttributeData)) != 0 )
    {
        m_valid = fileAttributeData.dwFileAttributes | FILE_ATTRIBUTE_DIRECTORY;
        //Could open handle here if needed
    }
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
MountPointWin::~MountPointWin()
{
    //if the constructor opens the handle we need to close it here
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
std::vector<std::filesystem::path> MountPointWin::ListFiles()
{
    std::vector<std::filesystem::path> paths;

    return paths;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
VFS::File MountPointWin::FileCreate(const std::filesystem::path& name)
{
    UNUSEDPARAM(name);

    return File();
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
VFS::File MountPointWin::DirectoryCreate(const std::filesystem::path& name)
{
    UNUSEDPARAM(name);

    return File();
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
VFS::File MountPointWin::OpenFile(const std::filesystem::path& name)
{
    UNUSEDPARAM(name);

    return File();
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
bool MountPointWin::FileExists(const std::filesystem::path& name)
{
    UNUSEDPARAM(name);

    return false;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
bool MountPointWin::DirectoryExists(const std::filesystem::path& name)
{
    UNUSEDPARAM(name);

    return false;
}

}