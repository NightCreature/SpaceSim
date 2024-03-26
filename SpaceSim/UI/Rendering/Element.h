#pragma once

#include "Core/tinyxml2.h"

class Resource;
struct CommandList;

namespace FE
{

class Message;

namespace Rendering
{

class Element
{
public:
    virtual ~Element() {}

    virtual void Deserialise(tinyxml2::XMLElement& element) = 0;
    virtual void Update() = 0;
    virtual void PopulateCommandList(Resource* resource, CommandList& list) = 0;
    virtual void HandleMessage(Message& msg) = 0;
};

}

}