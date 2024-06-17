#pragma once

#include "Gameplay/ECS/Component.h"
#include "Core/tinyxml2.h"

namespace ECS
{
    struct LockedToEntityPositionComponent : public Component
    {
        LockedToEntityPositionComponent() = default;
        virtual ~LockedToEntityPositionComponent() = default;

        DECLARE_COMPONENT(LockedToEntityPositionComponent);

        void Deserialise(const tinyxml2::XMLElement* element) override
        {
            const auto* attribute = element->FindAttribute("entity");
            if (attribute != nullptr)
            {
                m_entityName = attribute->Value();
            }
            if (m_entityName == "player")
            {
                m_entityID = 0;
            }
            else
            {
                //Lookup the entity ID from the name, this might need to go through the level object
            }
        }

        size_t m_entityID;
        std::string m_entityName;
    };
}