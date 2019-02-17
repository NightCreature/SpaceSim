#include "Core/FileSystem/FileSystem.h"

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
VFS::FileSystem::~FileSystem()
{
    for (auto mountPoint : m_mountPoints)
    {
        delete mountPoint;
    }

    m_mountPoints.clear();
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void VFS::FileSystem::AddMountPoint(MountPoint* mount)
{
    m_mountPoints.push_back(mount);
}

///-----------------------------------------------------------------------------
///! @brief  List the files we can find not recursive
///! @remark Expensive lists all files we know about in the mount points
///-----------------------------------------------------------------------------
std::vector<std::filesystem::path> VFS::FileSystem::ListFiles()
{
    std::vector<std::filesystem::path> files;
    for (auto mountPoint : m_mountPoints)
    {
        auto fileList = mountPoint->ListFiles();
        files.insert(files.end(), fileList.begin(), fileList.end());
    }

    return files;
}
