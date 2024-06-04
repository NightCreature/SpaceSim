#include "Logging/FileLogger.h"

#include "Core/Resource/GameResource.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "Logging/LoggingMacros.h"

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
FileLogger::FileLogger(const std::filesystem::path& logPath)
{
    std::filesystem::path logFile = logPath / "log.log";
    m_fileLog.open(logFile, std::ios_base::out | std::ios_base::trunc);

    if (!m_fileLog.is_open())
    {
        MSG_WARN_CHANNEL("FILE LOGGER", "Failed to open log file: %s", logFile.c_str());
    }
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
FileLogger::~FileLogger()
{
    m_fileLog.flush();
    m_fileLog.close();
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void FileLogger::LogMessage(const std::string& message)
{
    m_fileLog << message;
}
