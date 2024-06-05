#pragma once

#include "Math/vector2.h"
#include "Math/vector3.h"
#include "Math/matrix44.h"

class Bbox;

class Frustum
{
public:
    Frustum( const Matrix44& viewMatrix, const Matrix44& projectionMatrix);
    ~Frustum();

    bool IsInside(const Bbox& boundingBox) const;
    //bool CheckPoint(float, float, float);
    //bool CheckCube(float, float, float, float);
    //bool CheckSphere(float, float, float, float);
    //bool CheckRectangle(float, float, float, float, float, float);

private:

    struct ViewPlane
    {
       //Vector3 m_normal;
       //float m_distanceFromOrigin;
        float a, b, c, d;

       void NormalizePlane();

       enum class Halfspace
       {
           NEGATIVE = -1,
           ON_PLANE = 0,
           POSITIVE = 1,
       };
       float ClassifyPoint(const Vector3& pt) const;
/*
       void normalize()
       {
           float length = m_normal.length();
           if (length >  0.00001f)
           {
               float invLength = 1.0f / length;
               m_normal = m_normal * invLength;
               m_distanceFromOrigin *= invLength;
           }
       }*/
    };
    ViewPlane m_planes[6];
};

