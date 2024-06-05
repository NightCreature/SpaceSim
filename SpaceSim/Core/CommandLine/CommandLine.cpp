#include "CommandLine.h"

#include "Core/StringOperations/StringHelperFunctions.h"

bool CommandLine::HasCommand(const std::string& command) const
{
    HashString commandHash(command);
    for (const auto& token : m_tokens)
    {
        if (token.GetCommandHash() == commandHash)
        {
            return true;
        }
    }
    return false;
}

const CommandToken& CommandLine::GetCommand(const std::string& command) const
{
    CommandToken retVal;
    HashString commandHash(command);
    for (const auto& token : m_tokens)
    {
        if (token.GetCommandHash() == commandHash)
        {
            retVal = token;
        }
    }
    return retVal;
}

void CommandLine::SplitIntoTokens()
{
    //First split the command line on spaces
    auto tokens = tokeniseString(m_commandLine, ' ');
    //Then look for tokens with a - in front of them or a / in front of them or a \ in front of them
    for (size_t i = 0; i < tokens.size(); ++i)
    {
        if (tokens[i][0] == '-' || tokens[i][0] == '\\' || tokens[i][0] == '/')
        {
            std::string command = tokens[i].substr(1);
            std::string value = "";

            if (i + 1 < tokens.size() && tokens[i + 1][0] != '-' && tokens[i + 1][0] != '\\' && tokens[i + 1][0] != '/')
            {
                value = tokens[++i];
            }

            m_tokens.push_back(CommandToken(command, value));
        }
    }
}
