#pragma once 

#include "Core/tinyxml2.h"
#include "Gameplay/ECS/Component.h"


namespace ECS
{

struct MaterialRef : public Component
{
    MaterialRef() = default;
    virtual ~MaterialRef() = default;

    DECLARE_COMPONENT(MaterialRef);

    void Deserialise(const tinyxml2::XMLElement* element) override
    {
        m_materialName = element->Attribute("name");
    }

    std::string m_materialName;
};
}