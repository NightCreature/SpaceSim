#include "ElementManager.h"

#include "Element.h"
#include "ElementRegistry/ElementRegistry.h"

#include "Core/MessageSystem/MessageObserver.h"
#include "Core/Resource/RenderResource.h"

#include "FastDelegate.h"

#include "UI/Messages.h"

#include <memory>
#include "Core/Types/TypeHelpers.h"




namespace FE
{

namespace Rendering
{

void ElementManager::Initialise(Resource* resource)
{
    m_resource = resource;

    //Need to read in the art data and stuff

}

void ElementManager::Update()
{
    for (const auto& element : m_elements)
    {
        element->Update();
    }
}

void ElementManager::RegisterElementDispatchFuncions(MessageSystem::MessageObserver& observer)
{
    observer.AddDispatchFunction(MESSAGE_ID(CreateScreen), fastdelegate::MakeDelegate(this, &ElementManager::dispatchMessage));
    observer.AddDispatchFunction(MESSAGE_ID(SelectObjectMessage), fastdelegate::MakeDelegate(this, &ElementManager::dispatchMessage));
}

void ElementManager::dispatchMessage(const MessageSystem::Message& msg)
{
    switch (msg.getMessageId())
    {
    case "SelectObjectMessage"_hash:
        //This will dispatch to the active ScreenElement to further dispatch this
        break;

    case "CreateScreen"_hash:
        //This creates the needed elements, which are then in a table
        break;
    }
}

bool ElementManager::CreateElement(const std::string_view& type)
{
    UNUSEDPARAM(type);
    //Clang thinks something is scoping out here
    //auto& registry = GetElementRegistry();
    //auto* creationFunction = registry.m_idToCreationFp[type.data()];
    //if (creationFunction != nullptr)
    //{
    //    Element* element = creationFunction();
    //    if (element != nullptr)
    //    {
    //        m_elements.emplace_back(std::make_unique(element));
    //        return true;
    //    }
    //}

    return false;
}

}

}