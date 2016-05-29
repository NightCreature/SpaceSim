#pragma once

class SourceInfo
{
public:
    SourceInfo(const char* file_name, size_t line_number) : m_fileName(file_name), m_lineNumber(line_number) {}

private:
    std::string m_fileName;
    size_t m_lineNumber;
};