#pragma once

#include "Core/FileSystem/MountPoint.h"
#include "Core/FileSystem/File.h"

#include <filesystem>
#include <string>
#include <vector>

namespace VFS
{

class FileSystem
{
public:
    FileSystem() {}
    ~FileSystem();

    void AddMountPoint(MountPoint* mount);
    bool RemoveMountPoint(const std::filesystem::path& mount);

    //this all redirects to the mount points the filesystem is aware off.
    std::vector<std::filesystem::path>& ListFiles();
    File CreateFile(const std::filesystem::path& name);
    File CreateDirectory(const std::filesystem::path& name);
    File OpenFile(const std::filesystem::path& name);

    bool FileExists(const std::filesystem::path& name);
    bool DirectoryExists(const std::filesystem::path& name);
private:
    std::vector<MountPoint*> m_mountPoints;
};

}