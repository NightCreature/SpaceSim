#pragma once

class Resource;
class CommandList;

namespace FE
{

class Message;

namespace Rendering
{

class Element
{
public:
    virtual void Update() = 0;
    virtual void PopulateCommandList(Resource* resource, CommandList& list) = 0;
    virtual void HandleMessage(Message& msg) = 0;
};

}

}