#include "UI/Core/Behaviours/SelectObject.h"

#include "Core/tinyxml2.h"

namespace FE::Behaviours
{

constexpr std::string_view objectTag = "object";

void SelectObject::Serialise(const tinyxml2::XMLElement* element)
{
    const tinyxml2::XMLAttribute* objectAttribute = element->FindAttribute(objectTag.data());
    if (objectAttribute != nullptr)
    {
        m_object = objectAttribute->Value();
    }
}

void SelectObject::Activate()
{
    //Send object selection to render side
}

void SelectObject::Deactivate()
{

}

}
