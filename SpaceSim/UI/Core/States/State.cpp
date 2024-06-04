#include "State.h"

#include "Core/tinyxml2.h"

#include <string>

namespace FE
{

class Transition;

namespace States
{

constexpr std::string_view idAttributeName = "name";

bool State::Serialise(tinyxml2::XMLElement* element)
{
    const char* attributeId = element->Attribute(idAttributeName.data(), nullptr);
    if (attributeId != nullptr)
    {
        m_name = attributeId;
    }

    return true;
}

}

}