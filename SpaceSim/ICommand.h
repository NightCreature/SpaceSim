#pragma once

#include "GameResource.h"

#include <string>

struct ICommandData
{
    GameResource* m_resource;
#ifdef _DEBUG
    std::string m_commandName;
#endif
};

class ICommand
{
public:
    typedef void (CommandCallback)(ICommandData* data); //Use this if the command requires a return value when it is done, return values are configurable in the data

    ICommand(ICommandData* commandData, CommandCallback* callback) : m_commandData(commandData), m_callback(callback) {}
    virtual ~ICommand() {}

    virtual bool RunCommand() = 0;

#ifdef _DEBUG
    const std::string& getName() const { return m_commandData->m_commandName; }
#endif
protected:
    ICommandData* m_commandData;
    CommandCallback* m_callback;
};