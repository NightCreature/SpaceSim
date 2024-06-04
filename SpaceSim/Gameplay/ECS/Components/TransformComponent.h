#pragma once

#include "Math/matrix44.h"
#include "Gameplay/ECS/Component.h"

namespace ECS
{

struct TransformComponent : public Component
{
    DECLARE_COMPONENT(TransformComponent);

    Matrix44 m_transform;

};

}