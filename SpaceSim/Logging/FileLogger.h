#pragma once 

#include "Logging/Logger.h"
#include <fstream>

class Resource;

class FileLogger : public ILog
{
public:
    FileLogger(const std::string& logPath);
    virtual ~FileLogger();

    virtual void LogMessage(const std::string& message) override;

    bool is_open() const { return m_fileLog.is_open(); }
private:
    std::fstream m_fileLog;
};