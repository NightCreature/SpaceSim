#pragma once

#include "Core/StringOperations/HashString.h"

#include <string>

class CommandToken
{
public:
    CommandToken() = default; //default constructor
    CommandToken(const std::string& command, const std::string value) : m_command(command), m_value(value), m_commandHash(command) {}
    virtual ~CommandToken() = default;

    const std::string& GetCommand() const { return m_command; }
    const std::string& GetValue() const { return m_value; }
    const HashString& GetCommandHash() const { return m_commandHash; }

    virtual bool Execute() const {}
private:
    std::string m_command;
    std::string m_value;

    HashString m_commandHash = 0;
};