#pragma once

#include "Gameplay/ECS/Components/TransformComponent.h"
#include "Gameplay/ECS/Components/MaterialComponent.h"
#include "Gameplay/ECS/Components/PhysicsComponent.h"
#include "Gameplay/ECS/Components/MaterialRef.h"
#include "Gameplay/ECS/Components/ModelComponent.h"
#include "Gameplay/ECS/Components/LockedToPlayerPositionComponent.h"
#include "Gameplay/ECS/Components/LinkedEntityComponent.h"
#include "Gameplay/ECS/Components/FireLasersComponent.h"

namespace ECS
{

struct RenderObjectComponent : public Component
{
    DECLARE_COMPONENT(RenderObjectComponent);

    size_t m_renderObjectHandle = 0; //very unlikely we get a zero from a hash function
    MaterialConstants m_material;

    void Deserialise(const tinyxml2::XMLElement* element) override
    {
        throw std::logic_error("The method or operation is not implemented.");
    }
};

#define REGISTER_COMPONENT(ComponentName)\
RegisterComponent<ComponentName>(#ComponentName);

inline void RegisterComponents()
{
    REGISTER_COMPONENT(TransformationComponent);
    REGISTER_COMPONENT(RenderObjectComponent);
    REGISTER_COMPONENT(PhysicsComponent);
    REGISTER_COMPONENT(MaterialRef);
    REGISTER_COMPONENT(ModelComponent);
    REGISTER_COMPONENT(LockedToEntityPositionComponent);
    REGISTER_COMPONENT(FireLasersComponent);
    REGISTER_COMPONENT(LinkedEntityComponent);
}

}