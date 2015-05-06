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
    ICommand(ICommandData* commandData) : m_commandData(commandData) {}
    virtual ~ICommand() {}

    virtual bool RunCommand() = 0;

#ifdef _DEBUG
    const std::string& getName() const { return m_commandData->m_commandName; }
#endif
protected:
    ICommandData* m_commandData;
};