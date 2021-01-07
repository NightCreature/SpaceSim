#include "Core/FileSystem/File.h"

#include "Core/StringOperations/StringHelperFunctions.h"

#include <windows.h>


struct PlatformSpecficFileDataWin
{
    HANDLE m_fileHandle = INVALID_HANDLE_VALUE;
    size_t m_filePosition = 0;
    size_t m_fileSize = 0;
};

void VFS::File::Close()
{
    auto platformData = static_cast<PlatformSpecficFileDataWin*>(m_platformSpecificData);
    CloseHandle(platformData->m_fileHandle);
    platformData->m_filePosition = 0;
    platformData->m_fileSize = 0;
    delete platformData;
    m_platformSpecificData = nullptr;
}

void VFS::File::createFile(const std::filesystem::path& name, FileMode fileMode)
{
    m_name = name;

    DWORD creationDisposition;
    switch (fileMode)
    {
    case FileMode::Open:
        creationDisposition = OPEN_EXISTING;
        break;
    case FileMode::Create:
    case FileMode::OpenAndCreate:
        creationDisposition = CREATE_NEW;
        break;
    default:
        return;
    }

    HANDLE handle = CreateFile(name.string().c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, creationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE != handle)
    {
        auto platformData = new PlatformSpecficFileDataWin();
        platformData->m_fileHandle = handle;
        m_platformSpecificData = static_cast<void*>(platformData);
    }
    else
    {
        MSG_TRACE_CHANNEL("FILESYSTEM", "Creating file: %s failed for reason: %s", m_name.string().c_str(), getLastErrorMessage(GetLastError()));
    }
}

///-----------------------------------------------------------------------------
///! @brief 
///! @remark
///-----------------------------------------------------------------------------
void VFS::File::Write(byte* data, size_t length)
{
    if (data == nullptr || length <= 0)
    {
        return; //nothing to do no data or zero bytes to write
    }

    PlatformSpecficFileDataWin* platformData = static_cast<PlatformSpecficFileDataWin*>(m_platformSpecificData);

    if (m_platformSpecificData != nullptr && platformData->m_fileHandle != INVALID_HANDLE_VALUE)
    {
        size_t amountToWrite = length;
        while (length >= std::numeric_limits<unsigned long>::max())
        {
            if (!WriteFileEx(platformData->m_fileHandle, static_cast<void*>(data), std::numeric_limits<unsigned long>::max(), NULL, NULL))
            {
                MSG_TRACE_CHANNEL("FILESYSTEM", "Writing to file: %s failed for reason: %s", m_name.string().c_str(), getLastErrorMessage(GetLastError()));
                return;
            }
            amountToWrite -= std::numeric_limits<unsigned long>::max();
        }

        if (!WriteFileEx(platformData->m_fileHandle, static_cast<void*>(data), static_cast<unsigned long>(amountToWrite), NULL, NULL))
        {
            //Handle error file writing fucked up
            MSG_TRACE_CHANNEL("FILESYSTEM", "Writing to file: %s failed for reason: %s", m_name.string().c_str(), getLastErrorMessage(GetLastError()));
            return;
        }

        platformData->m_filePosition += length;
    }
}

///-----------------------------------------------------------------------------
///! @brief This reads the full file buffer, this function allocates a buffer on the provided pointer
///! @remark if this function fails it returns a nullptr, otherwise it returns the pointer to the copied data, the data input is the buffer the data will be written to
///-----------------------------------------------------------------------------
byte* VFS::File::Read(byte*& data)
{
    PlatformSpecficFileDataWin* platformData = static_cast<PlatformSpecficFileDataWin*>(m_platformSpecificData);
    if (data == nullptr || m_platformSpecificData == nullptr || platformData->m_fileHandle == INVALID_HANDLE_VALUE)
    {
        return nullptr;
    }

    LARGE_INTEGER  fileSize;
    if (!GetFileSizeEx(platformData->m_fileHandle, &fileSize))
    {
        MSG_TRACE_CHANNEL("FILESYSTEM", "Couldn't get file size for file: %s for reason: %s", m_name.string().c_str(), getLastErrorMessage(GetLastError()));
        return nullptr;//Couldnt get file size
    }

    platformData->m_fileSize = fileSize.QuadPart;
    data = new byte[platformData->m_fileSize];

    if (!ReadFileEx(platformData->m_fileHandle, static_cast<void*>(data), static_cast<unsigned long>(platformData->m_fileSize), NULL, NULL))
    {
        //Handle error file writing fucked up
        delete[] data;
        MSG_TRACE_CHANNEL("FILESYSTEM", "Couldn't Read from file: %s for reason: %s", m_name.string().c_str(), getLastErrorMessage(GetLastError()));
        return nullptr;
    }

    //set the file position to end of file
    platformData->m_filePosition = platformData->m_fileSize;

    return data;
}

///-----------------------------------------------------------------------------
///! @brief reads a certain amount from the current file position or until EOF, this function allocates a buffer on the provided pointer
///! @remark if this function fails it returns a nullptr, otherwise it returns the pointer to the copied data, the data input is the buffer the data will be written to
///-----------------------------------------------------------------------------
byte* VFS::File::Read(byte*& data, size_t amount)
{
    PlatformSpecficFileDataWin* platformData = static_cast<PlatformSpecficFileDataWin*>(m_platformSpecificData);
    if (data == nullptr || amount <= 0 || platformData == nullptr || platformData->m_fileHandle == INVALID_HANDLE_VALUE)
    {
        return nullptr; //nothing to read or nowhere to move the data to
    }

    LARGE_INTEGER  fileSize = { 0 };
    if (!GetFileSizeEx(platformData->m_fileHandle, &fileSize))
    {
        MSG_TRACE_CHANNEL("FILESYSTEM", "Couldn't get file size for file: %s for reason: %s", m_name.string().c_str(), getLastErrorMessage(GetLastError()));
        return nullptr;//Couldnt get file size
    }

    //This should be done with an overlapped structure in the end
    platformData->m_fileSize = fileSize.QuadPart;
    byte* fileData = new byte[platformData->m_fileSize];

    if (!ReadFileEx(platformData->m_fileHandle, static_cast<void*>(fileData), static_cast<unsigned long>(platformData->m_fileSize), NULL, NULL))
    {
        //Handle error file writing fucked up
        delete[] fileData;
        MSG_TRACE_CHANNEL("FILESYSTEM", "Couldn't Read from file: %s for reason: %s", m_name.string().c_str(), getLastErrorMessage(GetLastError()));
        return nullptr;
    }

    size_t amountToCopy = amount;
    if ((platformData->m_filePosition + amount) >= platformData->m_fileSize)
    {
        amountToCopy = amount - ((platformData->m_filePosition + amount) - platformData->m_fileSize);
    }
    
    data = new byte[amountToCopy];
    memcpy(static_cast<void*>(data), static_cast<void*>(&(fileData[platformData->m_filePosition])), amountToCopy);
    platformData->m_filePosition += amountToCopy;

    delete[] fileData;

    return data;
}

