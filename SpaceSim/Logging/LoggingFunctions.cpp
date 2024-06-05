#include "Logging/LoggingFunctions.h"

#include "Application/BaseApplication.h"
#include "Logger.h"


#define WIN32_LEAN_AND_MEAN
#include <windows.h>

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
//void debugOutput(TraceSeverity severity, const std::string& prefix, const char* file, int line, const char * format, ...)
//{
//    char buf[4096];
//    va_list args;
//    va_start(args, format);
//    vsprintf_s(buf, format, args);
//    va_end(args);
//    char debugOutputStr[5120];
//    std::string outputFormatString = "";
//    std::string prefixInternal = prefix;
//    if (prefix.empty())
//    {
//        prefixInternal = "TRACE";
//    }
//    prefixInternal = toUpperCase(prefixInternal);
//    
//    switch (severity)
//    {
//    default:
//    case TraceSeverity::EDEBUG:
//    case TraceSeverity::ELOG:
//        outputFormatString = "%s(%d): [%s]  %s\n";
//        break;
//    case TraceSeverity::EWARN:
//
//        outputFormatString = "%s(%d): [%s] WARNING : %s\n";
//        break;
//    case TraceSeverity::EERROR:
//
//        outputFormatString = "%s(%d): [%s] : ERROR : %s\n";
//        break;
//    case TraceSeverity::EASSERT:
//
//        outputFormatString = "%s(%d): [%s] : ASSERT : %s\n";
//        break;
//    }
//    sprintf_s(debugOutputStr, 5120, outputFormatString.c_str(), file, line, prefixInternal.c_str(), buf);
//    Application::m_logger.LogMessage(debugOutputStr);
//}

void LogMessage(const std::string& msg)
{
    Application::m_logger.LogMessage(msg);
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void debugOutput(TraceSeverity severity, const std::string& prefix, const std::source_location& location, const char* format, ...)
{
    char buf[4096];
    va_list args;
    va_start(args, format);
    vsprintf_s(buf, format, args);
    va_end(args);
    char debugOutputStr[5120];
    std::string outputFormatString = "";
    std::string prefixInternal;
    if (prefix.empty())
    {
        prefixInternal = "TRACE";
    }
    else
    {
        prefixInternal = toUpperCase(prefix);
    }

    switch (severity)
    {
    default:
    case TraceSeverity::EDEBUG:
    case TraceSeverity::ELOG:
        outputFormatString = "%s(%d): [%s]  %s\n";
        break;
    case TraceSeverity::EWARN:

        outputFormatString = "%s(%d): [%s] WARNING : %s\n";
        break;
    case TraceSeverity::EERROR:

        outputFormatString = "%s(%d): [%s] : ERROR : %s\n";
        break;
    case TraceSeverity::EASSERT:

        outputFormatString = "%s(%d): [%s] : ASSERT : %s\n";
        break;
    }
    sprintf_s(debugOutputStr, 5120, outputFormatString.c_str(), location.file_name(), location.line(), prefixInternal.c_str(), buf);
    Application::m_logger.LogMessage(debugOutputStr);
}