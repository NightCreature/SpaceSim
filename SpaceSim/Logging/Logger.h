#pragma once 

#include <mutex>
#include <string>
#include <vector>

class HttpMessageBuffer;

class ILog
{
public:
    ILog() {}
    virtual ~ILog() {}

    virtual void LogMessage(const std::string& message) = 0;
};

static std::mutex logMutex;

class OutputDebugLog : public ILog
{
public:
    OutputDebugLog() {}
    ~OutputDebugLog() {}

    virtual void LogMessage(const std::string& message) override;
};

class HttpDebugLog : public ILog
{
public:
    HttpDebugLog(HttpMessageBuffer* httpMessageBuffer) : m_httpMessageBuffer(httpMessageBuffer)
    {
        m_buffer.reserve(25); //reserve space for 25 lines
    }
    ~HttpDebugLog() {}

    virtual void LogMessage(const std::string& message) override;

private:
    std::vector< std::string > m_buffer;
    HttpMessageBuffer* m_httpMessageBuffer;
};

class Logger
{
public:
    Logger() {}
    ~Logger()
    {
        for (auto logger : m_logs)
        {
            delete logger;
        }
    }

    void addLogger(ILog* logger) { m_logs.push_back(logger);  }
    void removeLogger(ILog* logger) 
    {
        for (std::vector<ILog*>::iterator logIt = m_logs.begin();  logIt != m_logs.end(); ++logIt)
        {
            if (logger == *logIt)
            {
                ILog* instance = *logIt;
                m_logs.erase(logIt);

                delete instance;
                break;
            }
        }
    }

    void LogMessage(const std::string& message)
    {

        std::scoped_lock<std::mutex> lock(logMutex);
        for (auto logger : m_logs)
        {
            logger->LogMessage(message);
        }
    }

private:
    std::vector< ILog*> m_logs;
};