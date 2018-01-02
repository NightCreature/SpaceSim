#pragma once

class SourceInfo
{
public:
    explicit SourceInfo() {}
    SourceInfo(const char* file_name, size_t line_number) : m_fileName(file_name), m_lineNumber(line_number) {}

    const std::string& getSourceFileName() const { return m_fileName; }
    size_t getSourceFileLineNumber() const { return m_lineNumber; }

private:
    std::string m_fileName;
    size_t m_lineNumber;
};