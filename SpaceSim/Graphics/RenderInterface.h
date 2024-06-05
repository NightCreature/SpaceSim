#pragma once
#include "Core/MessageSystem/GameMessages.h"

struct CommandList;
class Resource;

class RenderInterface
{
public:
    virtual ~RenderInterface() {}
    virtual void UpdateCbs() = 0;
    virtual void PopulateCommandlist(Resource* resource, CommandList& commandList) = 0;
};

class DebugRenderInterface : public RenderInterface
{
public:
    virtual void Update(const MessageSystem::RenderInformation::RenderInfo& context) = 0;
};