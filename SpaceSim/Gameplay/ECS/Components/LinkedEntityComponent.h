#pragma once

#include "Gameplay/ECS/Component.h"
#include <string>

namespace ECS
{
    struct LinkedEntityComponent : public Component
    {
        LinkedEntityComponent() = default;
        virtual ~LinkedEntityComponent() = default;

        DECLARE_COMPONENT(LinkedEntityComponent);

        void Deserialise(const tinyxml2::XMLElement* element) override
        {
            //attirbute of the element are: name = "LeftDoor" action = "activate"
            const auto* attribute = element->FindAttribute("name");
            if (attribute != nullptr)
            {
                m_entityName = attribute->Value();
            }
            attribute = element->FindAttribute("action");
            if (attribute != nullptr)
            {
                m_action = attribute->Value();
            }
        }

        std::string m_entityName;
        std::string m_action; //This should be something like a message or action defined in the game
    };
}