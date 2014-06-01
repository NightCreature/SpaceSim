#ifndef MATRIXMATH_H
#define MATRIXMATH_H

#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix33.h"
#include "matrix44.h"

#include <cmath>

namespace math
{
	const float gmPI = 3.14159265f;
	float toRadian(float degree);
	float toDegree(float radian);
	template <class T>
	T lerp(const T &a, const T &b, const float pos) 
	{
		return pos * a + (1 - pos) * b;
	}

    inline Matrix44 createLeftHandedFOVPerspectiveMatrix(float fieldOfViewY, float aspectRatio, float znearPlane, float zfarPlane)
    {
        float height = 1.0f / tan(fieldOfViewY/2);
        float width =  height / aspectRatio;
        return Matrix44(width, 0.0f, 0.0f, 0.0f,
                        0.0f,  height, 0.0f, 0.0f,
                        0.0f, 0.0f, zfarPlane/(zfarPlane-znearPlane), 1.0f,
                        0.0f, 0.0f, -znearPlane*zfarPlane/(zfarPlane-znearPlane), 0.0f);
    }
}
#endif