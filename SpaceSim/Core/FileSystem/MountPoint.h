#pragma once

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
    MountPoint(const std::filesystem::path path) : m_rootPath(path), m_valid(false) {}
    virtual ~MountPoint() {}

    bool IsValid() { return m_valid; }

    virtual std::vector<std::filesystem::path> ListFiles() = 0;
    virtual File FileCreate(const std::filesystem::path& name) = 0;
    virtual File DirectoryCreate(const std::filesystem::path& name) = 0;
    virtual File OpenFile(const std::filesystem::path& name) = 0;

    virtual bool FileExists(const std::filesystem::path& name) = 0;
    virtual bool DirectoryExists(const std::filesystem::path& name) = 0;
protected:
    std::filesystem::path m_rootPath;
    bool m_valid;
};

}