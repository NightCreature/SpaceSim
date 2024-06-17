#pragma once
#include "Gameplay/ECS/Component.h"

namespace ECS
{

struct ModelComponent : public Component
{
    DECLARE_COMPONENT(ModelComponent);

    std::filesystem::path m_modelPath;

    void Deserialise(const tinyxml2::XMLElement* element) override
    {
        m_modelPath = element->Attribute("file_name");
    }
};

}