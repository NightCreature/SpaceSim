#pragma once

#include "Core/FileSystem/File.h"
#include "Core/FileSystem/Flags.h"
#include "Core/FileSystem/MountPoint.h"
#include "Core/Paths.h"

#include <filesystem>
#include <string>
#include <vector>

#if defined CreateFile
#undef CreateFile
#endif

namespace VFS
{

class FileSystem
{
public:
    FileSystem(const Paths& paths);
    ~FileSystem();

    void AddMountPoint(const MountPoint& mount);
    bool RemoveMountPoint(const std::filesystem::path& mount);
    bool RemoveMountPoint(const MountPoint& mount);

    //this all redirects to the mount points the filesystem is aware off.
    std::vector<std::filesystem::path> ListFiles();
    std::vector<std::filesystem::path> ListFiles(const std::filesystem::path& path);
    File CreateFile(const std::filesystem::path& name, FileMode fileMode);
    File CreateDirectory(const std::filesystem::path& name);

    bool FileExists(const std::filesystem::path& name);
    bool DirectoryExists(const std::filesystem::path& name);
private:

    std::vector<MountPoint> m_mountPoints;
    Paths m_paths;
};

}