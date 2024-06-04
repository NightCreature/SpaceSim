#pragma once
#include <source_location>

class SourceInfo
{
public:
    SourceInfo(const std::source_location& location = std::source_location::current()) : m_sourceInfo(location) {}

    const std::string& getSourceFileName() const { return m_sourceInfo.file_name(); }
    size_t getSourceFileLineNumber() const { return m_sourceInfo.line(); }

private:
    std::source_location m_sourceInfo;
};