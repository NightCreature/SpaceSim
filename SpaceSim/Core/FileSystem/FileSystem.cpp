#include "Core/FileSystem/FileSystem.h"

#include "Core/Platform/Defines.h"
#include "Core/Profiler/ProfilerMacros.h"

#if defined CreateFile
#undef CreateFile
#endif

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
VFS::FileSystem::FileSystem(const Paths& paths) : m_paths(paths)
{
    AddMountPoint(MountPoint(m_paths.getEffectShaderPath()));
    AddMountPoint(MountPoint(m_paths.getLogPath()));
    AddMountPoint(MountPoint(m_paths.getModelPath()));
    AddMountPoint(MountPoint(m_paths.getScenePath()));
    AddMountPoint(MountPoint(m_paths.getSettingsPath()));
    AddMountPoint(MountPoint(m_paths.getTexturesPath()));
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
VFS::FileSystem::~FileSystem()
{
    m_mountPoints.clear();
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void VFS::FileSystem::AddMountPoint(const MountPoint& mount)
{
    m_mountPoints.push_back(mount);
}

///-----------------------------------------------------------------------------
///! @brief  List the files we can find not recursive
///! @remark Expensive lists all files we know about in the mount points
///-----------------------------------------------------------------------------
std::vector<std::filesystem::path> VFS::FileSystem::ListFiles()
{
    PROFILE_FUNCTION();

    std::vector<std::filesystem::path> files;
    for (auto mountPoint : m_mountPoints)
    {
        auto fileList = mountPoint.ListFiles();
        files.insert(files.end(), fileList.begin(), fileList.end());
    }

    return files;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
VFS::File VFS::FileSystem::CreateFile(const std::filesystem::path& name, FileMode createOrOpen)
{
        //have to go through the mount points to see where it should be
    for (auto& mountPoint : m_mountPoints)
    {
        if (mountPoint.IsRootPathOf(name.parent_path()))
        {
            return mountPoint.FileCreate(name, createOrOpen);
        }
    }

    return File();
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
std::vector<std::filesystem::path> VFS::FileSystem::ListFiles(const std::filesystem::path& path)
{
    std::vector<std::filesystem::path> files;
    for (auto mountPoint : m_mountPoints)
    {
        auto fileList = mountPoint.ListFiles(path);
        files.insert(files.end(), fileList.begin(), fileList.end());
    }

    return files;
}