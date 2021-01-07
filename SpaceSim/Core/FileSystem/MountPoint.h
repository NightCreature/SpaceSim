#pragma once

#include "Core/FileSystem/Flags.h"

#include <filesystem>
#include <string>
#include <vector>


namespace VFS
{
class File;

///-----------------------------------------------------------------------------
///! @brief Represents a file provider interface, aka a filesystem directory of a specific platform
///! @remark Implementations are platform specific, path in constructor is a full path that has a root in the current platforms filesystem
///-----------------------------------------------------------------------------
class MountPoint
{
public:
    MountPoint(const std::filesystem::path path) : m_rootPath(path), m_valid(false) { PlatformSpecificConstruct(); }
    ~MountPoint() {}

    bool IsValid() { return m_valid; }

    std::vector<std::filesystem::path> ListFiles();
    File FileCreate(const std::filesystem::path& name, FileMode fileMode);
    File DirectoryCreate(const std::filesystem::path& name);

    bool FileExists(const std::filesystem::path& name);
    bool DirectoryExists(const std::filesystem::path& name);

    bool IsRootPathOf(const std::filesystem::path& path) const;
protected:
    void PlatformSpecificConstruct();

    std::filesystem::path m_rootPath;
    bool m_valid;
};

///-----------------------------------------------------------------------------
///! @brief   Checks whether this mount point should represent the given full path
///! @remark 
///-----------------------------------------------------------------------------
inline bool MountPoint::IsRootPathOf(const std::filesystem::path& path) const
{
    return m_rootPath <= path;
}

}