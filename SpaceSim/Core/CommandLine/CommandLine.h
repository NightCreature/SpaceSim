#pragma once

#include "Core/CommandLine/CommandToken.h"

#include <string>
#include <vector>

class CommandLine
{
public:
    CommandLine() = default; //default constructor
    CommandLine(const std::string& commandLine) : m_commandLine(commandLine) { SplitIntoTokens(); }

    bool HasCommand(const std::string& command) const;
    const CommandToken& GetCommand(const std::string& command) const;

private:
    void SplitIntoTokens();

    std::string m_commandLine;
    std::vector<CommandToken> m_tokens;
};
