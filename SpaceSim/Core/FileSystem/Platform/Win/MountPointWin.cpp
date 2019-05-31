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
    
    if (m_rootPath.is_absolute())
    {
        //make sure this path actually exists
        WIN32_FILE_ATTRIBUTE_DATA fileAttributeData;
        ZeroMemory(&fileAttributeData, sizeof(WIN32_FILE_ATTRIBUTE_DATA));
        if (GetFileAttributesEx(path.string().c_str(), GetFileExInfoStandard, static_cast<void*>(&fileAttributeData)) != 0)
        {
            m_valid = fileAttributeData.dwFileAttributes | FILE_ATTRIBUTE_DIRECTORY;
            //Could open handle here if needed
        }
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


    WIN32_FIND_DATA ffd;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    // Find the first file in the directory.
    auto searchPath = m_rootPath / "*"; 
    hFind = FindFirstFile(searchPath.string().c_str(), &ffd);

    if (INVALID_HANDLE_VALUE != hFind)
    {
        paths.push_back(std::filesystem::path(ffd.cFileName));
        do
        {
            paths.push_back(std::filesystem::path(ffd.cFileName));
        }
        while (FindNextFile(hFind, &ffd) != 0);

    }

    FindClose(hFind);

    return paths;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
VFS::File* MountPointWin::FileCreate(const std::filesystem::path& name)
{
    UNUSEDPARAM(name);

    return nullptr;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
VFS::File* MountPointWin::DirectoryCreate(const std::filesystem::path& name)
{
    UNUSEDPARAM(name);

    return nullptr;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
VFS::File* MountPointWin::OpenFile(const std::filesystem::path& name)
{
    UNUSEDPARAM(name);

    //This opens a file seeing the file is an interface too 

    return nullptr;
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