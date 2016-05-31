#pragma once 

#include "Logging/Logger.h"
#include <fstream>

class FileLogger : public ILog
{
public:
    FileLogger(const std::string& logPath);
    virtual ~FileLogger();

    virtual void LogMessage(const std::string& message) override;
private:
    std::fstream m_fileLog;
};