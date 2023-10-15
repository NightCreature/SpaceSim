#pragma once

#include "Element.h"

#include <vector>
#include <memory>
#include "Core/StringOperations/HashString.h"


class Resource;
class CommandList;

namespace FE
{

class Message;

namespace Rendering
{

class ElementManager
{
public:
    ElementManager() = default;
    ~ElementManager() = default;
    void Initialise(Resource* resource);
    void Update();
    void HandleMessage(Message& msg); //This should respond to create element messages
private:
    // Need to register the types, look at component and others
    bool CreateElement(const std::string_view& type);

    Resource* m_resource;

    std::vector<std::unique_ptr<Element>> m_elements;
};

}

}