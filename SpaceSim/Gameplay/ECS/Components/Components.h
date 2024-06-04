#pragma once

#include "Gameplay/ECS/Components/TransformComponent.h"
#include "Gameplay/ECS/Components/MaterialComponent.h"
#include "Gameplay/ECS/Components/PhysicsComponent.h"

namespace ECS
{

struct RenderObjectComponent : public Component
{
    DECLARE_COMPONENT(RenderObjectComponent);

    size_t m_renderObjectHandle = 0; //very unlikely we get a zero from a hash function
    MaterialConstants m_material;
};

#define REGISTER_COMPONENT(ComponentName)\
RegisterComponent<ComponentName>(#ComponentName);

inline void RegisterComponents()
{
    REGISTER_COMPONENT(TransformComponent);
    REGISTER_COMPONENT(RenderObjectComponent);
    REGISTER_COMPONENT(PhysicsComponent);
}

}