#pragma once

#include "Element.h"

#include "Core/MessageSystem/Messages.h"
#include "Core/StringOperations/HashString.h"

#include <vector>
#include <memory>


class Resource;
struct CommandList;

namespace MessageSystem
{ 
class MessageObserver;
class Message;
}

namespace FE
{

class Message;

namespace Rendering
{

class ElementManager : public MessageSystem::IMessageDispatcher
{
public:
    ElementManager() = default;
    ~ElementManager() = default;
    void Initialise(Resource* resource);
    void Update();

    void RegisterElementDispatchFuncions(MessageSystem::MessageObserver& observer);

    void dispatchMessage(const MessageSystem::Message& msg) override;

private:
    // Need to register the types, look at component and others
    bool CreateElement(const std::string_view& type);

    Resource* m_resource;

    std::vector<std::unique_ptr<Element>> m_elements;
};

}

}