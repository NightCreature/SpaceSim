#pragma once

#include <algorithm>
#include <fstream>
#include <string>

namespace Profiling
{

class EventDescriptor
{
public:
    EventDescriptor(const std::string& name, const std::string& fileName, const std::string& functionName, size_t lineNumber)
        : m_name(name)
        , m_fileName(fileName)
        , m_functionName(functionName)
        , m_lineNumber(lineNumber)
        , m_hash(0)
    {
        std::replace(m_fileName.begin(), m_fileName.end(), '\\', '/');
    }

    size_t computeHash() const;

    void toJson(std::fstream& stream);

    const std::string& getEventName() const { return m_name; }
    const std::string& getFileName() const { return m_fileName; }
    const std::string& getFunctioName() const { return m_functionName; }
    size_t getLineNumber() const { return m_lineNumber; }
    size_t getHash() const { return m_hash; }
private:
    std::string m_name;
    std::string m_fileName;
    std::string m_functionName;
    size_t m_lineNumber;
    mutable size_t m_hash;
};

}