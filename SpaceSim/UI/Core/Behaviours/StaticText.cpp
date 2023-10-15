#include "UI/Core/Behaviours/StaticText.h"

#include "Core/tinyxml2.h"

#include <string>

namespace FE
{

namespace Behaviours
{

constexpr std::string_view textTag = "text";

void StaticText::Serialise(const tinyxml2::XMLElement* element)
{
    for (const tinyxml2::XMLAttribute* attribute = element->FirstAttribute(); attribute != nullptr; attribute = attribute->Next())
    {
        if (attribute->Name() == textTag)
        {
            m_locId = attribute->Value();
        }
    }
}

void StaticText::Update(float deltaT, const InputState& input)
{
}

bool StaticText::HandleInput(const InputState& state)
{
    return false;
}

const Transition* StaticText::GetTransitionLink()
{
    return nullptr;
}

void StaticText::Activate()
{
    //Send locid off to render here
}

void StaticText::Deactivate()
{
    
}

}

}