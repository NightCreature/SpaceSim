#pragma once 

#include "Gameplay/ECS/Component.h"

namespace ECS
{
    struct FireLasersComponent : public Component
    {
        FireLasersComponent() = default;
        virtual ~FireLasersComponent() = default;

        DECLARE_COMPONENT(FireLasersComponent);

        void Deserialise(const tinyxml2::XMLElement* element) override
        {
            //THis is a tag component, it has no data, it might get a direction
            //fire_rate = "10" damager = "5"

            const auto* attribute = element->FindAttribute("fire_rate");
            if (attribute != nullptr)
            {
                m_fireRate = attribute->IntValue();
            }
            attribute = element->FindAttribute("damage");
            if (attribute != nullptr)
            {
                m_damage = attribute->IntValue();
            }
        }

        size_t m_fireRate = 10; //shots per second
        size_t m_damage = 5; //damage per shot
    };
}