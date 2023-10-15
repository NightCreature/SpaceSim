#include "ElementManager.h"

#include "Element.h"
#include "ElementRegistry/ElementRegistry.h"

#include <memory>

namespace FE
{

namespace Rendering
{

void ElementManager::Initialise(Resource* resource)
{

}

void ElementManager::Update()
{
    for (const auto& element : m_elements)
    {
        element->Update();
    }
}

void ElementManager::HandleMessage(Message& msg)
{

}

bool ElementManager::CreateElement(const std::string_view& type)
{
    auto& registry = GetElementRegistry();
    auto* creationFunction = registry.m_idToCreationFp[type.data()];
    if (creationFunction != nullptr)
    {
        Element* element = creationFunction();
        if (element != nullptr)
        {
            m_elements.push_back(std::unique_ptr<Element>(element));
            return true;
        }
    }

    return false;
}

}

}