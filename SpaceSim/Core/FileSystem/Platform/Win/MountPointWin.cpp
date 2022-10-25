#include "Core/FileSystem/MountPoint.h"

#include "Core/FileSystem/File.h"
#include "Core/Types/TypeHelpers.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace VFS
{

///-----------------------------------------------------------------------------
///! @brief Represents a directory on the Windows filesystem
///! @remark 
///-----------------------------------------------------------------------------
void MountPoint::PlatformSpecificConstruct()
{
    if (m_rootPath.is_absolute())
    {
        //make sure this path actually exists
        WIN32_FILE_ATTRIBUTE_DATA fileAttributeData;
        ZeroMemory(&fileAttributeData, sizeof(WIN32_FILE_ATTRIBUTE_DATA));
        if (GetFileAttributesEx(m_rootPath.string().c_str(), GetFileExInfoStandard, static_cast<void*>(&fileAttributeData)) != 0)
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
std::vector<std::filesystem::path> MountPoint::ListFiles()
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
VFS::File MountPoint::FileCreate(const std::filesystem::path& name, FileMode fileMode)
{
    File file;

    file.createFile(name, fileMode);

    return file;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
VFS::File MountPoint::DirectoryCreate(const std::filesystem::path& name)
{
    UNUSEDPARAM(name);

    return File();
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
bool MountPoint::FileExists(const std::filesystem::path& name)
{
    std::filesystem::path fileName = name;
    if (!name.is_absolute())
    {
        //This might cause problems you have to merge paths here
        fileName = m_rootPath / name;
    }
    
    //make sure this path actually exists
    WIN32_FILE_ATTRIBUTE_DATA fileAttributeData;
    ZeroMemory(&fileAttributeData, sizeof(WIN32_FILE_ATTRIBUTE_DATA));
    if (GetFileAttributesEx(fileName.string().c_str(), GetFileExInfoStandard, static_cast<void*>(&fileAttributeData)) != 0)
    {
        return fileAttributeData.dwFileAttributes != INVALID_FILE_ATTRIBUTES;
    }

    return false;
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
bool MountPoint::DirectoryExists(const std::filesystem::path& name)
{
    UNUSEDPARAM(name);

    return false;
}

}