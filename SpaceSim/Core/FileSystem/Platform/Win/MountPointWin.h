#pragma once

#include "Core/FileSystem/MountPoint.h"

#include <filesystem>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include "windows.h"


namespace VFS
{

class File;

class MountPointWin : public MountPoint
{
    
public:
    MountPointWin(const std::filesystem::path& path);
    virtual ~MountPointWin();

    virtual std::vector<std::filesystem::path> ListFiles() override;
    virtual File FileCreate(const std::filesystem::path& name) override;
    virtual File DirectoryCreate(const std::filesystem::path& name) override;
    virtual File OpenFile(const std::filesystem::path& name) override;
    virtual bool FileExists(const std::filesystem::path& name) override;
    virtual bool DirectoryExists(const std::filesystem::path& name) override;

private:
    HANDLE m_directoryHandle;
};

}