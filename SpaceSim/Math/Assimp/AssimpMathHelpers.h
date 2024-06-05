#pragma once

#include "Math/matrix44.h"

#include <assimp/matrix4x4.h>

namespace AssimpHelpers
{
    inline Matrix44 ToMatrix(const aiMatrix4x4& mat)
    {
        return Matrix44(
            mat.a1, mat.a2, mat.a3, mat.a4,
            mat.b1, mat.b2, mat.b3, mat.b4,
            mat.c1, mat.c2, mat.c3, mat.c4,
            mat.d1, mat.d2, mat.d3, mat.d4);
    }

    inline Vector4 ToVector(const aiVector3D& vec)
    {
        return Vector4(vec.x, vec.y, vec.z, 1);
    }
}