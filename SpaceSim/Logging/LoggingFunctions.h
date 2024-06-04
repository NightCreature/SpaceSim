#pragma once

#include <source_location>
#include <string>
#include <string_view>

void LogMessage(const std::string& msg);

enum class TraceSeverity : size_t
{
    EDEBUG,
    ELOG,
    EWARN,
    EERROR,
    EASSERT
};

//This is first so we can use the macros in debugging the functions below
//void debugOutput(TraceSeverity severity, const std::string& prefix, const char* file, int line, const char* format, ...);
void debugOutput(TraceSeverity severity, const std::string& prefix, const std::source_location& location, const char* format, ...);

std::string toUpperCase(const std::string_view& str);

inline const std::string GetCapitalizedPrefix(const std::string& prefix) 
{
    std::string prefixInternal;
    if (prefix.empty())
    {
        prefixInternal = "TRACE";
    }
    else
    {
        prefixInternal = toUpperCase(prefix);
    }

    return prefixInternal;
}

inline constexpr std::string_view GetErrorTypeMessage(TraceSeverity severity)
{
    switch (severity)
    {
    default:
    case TraceSeverity::EDEBUG:
    case TraceSeverity::ELOG:
        return "";
        break;
    case TraceSeverity::EWARN:
        return "WARNING:";
        break;
    case TraceSeverity::EERROR:
        return "ERROR:";
        break;
    case TraceSeverity::EASSERT:
        return "ASSERT:";
        break;
    }
}

template<class ...T>
void debugOutputFmt(TraceSeverity severity, const std::string& prefix, const std::source_location& location, const char* format, T&&... args)
{
    std::string formattedMsg = fmt::format(fmt::runtime(format), args...);

    const std::string capitalizedPrefix = GetCapitalizedPrefix(prefix);
    const std::string_view errorTypeMsg = GetErrorTypeMessage(severity);

    std::string outputMsg = fmt::format("{}({}): [{}] {} {}\n", location.file_name(), location.line(), capitalizedPrefix, errorTypeMsg, formattedMsg);
    LogMessage(outputMsg);
}