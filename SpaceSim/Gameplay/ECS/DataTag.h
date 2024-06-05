#pragma once

#include "Math/vector4.h"

namespace ECS
{

struct DataTag
{
};

struct TransformTag : public DataTag
{
    Vector4 m_position;
    Vector4 m_rotation;
    Vector4 m_scale;
};

}