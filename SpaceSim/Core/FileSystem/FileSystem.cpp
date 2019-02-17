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
