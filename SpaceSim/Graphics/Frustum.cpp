#include "Graphics/Frustum.h"

#include "Gameplay/bbox.h"

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
Frustum::Frustum( const Matrix44& viewMatrix, const Matrix44& projectionMatrix)
{

    Matrix44 ViewProjection = viewMatrix * projectionMatrix;

    //m_planes[0].m_normal = Vector3( ViewProjection[0][2] - ViewProjection[0][0],
    //                                  ViewProjection[1][3] - ViewProjection[1][0],
    //                                  ViewProjection[2][3] - ViewProjection[2][0]);
    //m_planes[0].m_distanceFromOrigin = ViewProjection[3][3] - ViewProjection[3][0];
    //m_planes[0].normalize();

    //m_planes[1].m_normal = Vector3( ViewProjection[0][3] + ViewProjection[0][0],
    //                                  ViewProjection[1][3] + ViewProjection[1][0],
    //                                  ViewProjection[2][3] + ViewProjection[2][0]);
    //m_planes[1].m_distanceFromOrigin = ViewProjection[3][3] + ViewProjection[3][0];
    //m_planes[1].normalize();

    //m_planes[2].m_normal = Vector3( ViewProjection[0][3] - ViewProjection[0][1],
    //                                  ViewProjection[1][3] - ViewProjection[1][1],
    //                                  ViewProjection[2][3] - ViewProjection[2][1]);
    //m_planes[2].m_distanceFromOrigin = ViewProjection[3][3] - ViewProjection[3][1];
    //m_planes[2].normalize();

    //m_planes[3].m_normal = Vector3( ViewProjection[0][3]+ViewProjection[0][1],
    //                                  ViewProjection[1][3]+ViewProjection[1][1],
    //                                  ViewProjection[2][3]+ViewProjection[2][1]);
    //m_planes[3].m_distanceFromOrigin = ViewProjection[3][3]+ViewProjection[3][1];
    //m_planes[3].normalize();

    //m_planes[4].m_normal = Vector3( ViewProjection[0][3] - ViewProjection[0][2],
    //                                  ViewProjection[1][3] - ViewProjection[1][2],
    //                                  ViewProjection[2][3] - ViewProjection[2][2]);
    //m_planes[4].m_distanceFromOrigin = ViewProjection[3][3] - ViewProjection[3][2];
    //m_planes[4].normalize();

    //m_planes[5].m_normal = Vector3( ViewProjection[0][3] + ViewProjection[0][2],
    //                                  ViewProjection[1][3] + ViewProjection[1][2],
    //                                  ViewProjection[2][3] + ViewProjection[2][2]);
    //m_planes[5].m_distanceFromOrigin = ViewProjection[3][3] + ViewProjection[3][2];
    //m_planes[5].normalize();

    // Left clipping plane
    m_planes[0].a = ViewProjection[0][3] + ViewProjection[0][0];
    m_planes[0].b = ViewProjection[1][3] + ViewProjection[1][0];
    m_planes[0].c = ViewProjection[2][3] + ViewProjection[2][0];
    m_planes[0].d = ViewProjection[3][3] + ViewProjection[3][0];
    // Right clipping plane
    m_planes[1].a = ViewProjection[0][3] - ViewProjection[0][0];
    m_planes[1].b = ViewProjection[1][3] - ViewProjection[1][0];
    m_planes[1].c = ViewProjection[2][3] - ViewProjection[2][0];
    m_planes[1].d = ViewProjection[3][3] - ViewProjection[3][0];
    // Top clipping plane
    m_planes[2].a = ViewProjection[0][3] - ViewProjection[0][1];
    m_planes[2].b = ViewProjection[1][3] - ViewProjection[1][1];
    m_planes[2].c = ViewProjection[2][3] - ViewProjection[2][1];
    m_planes[2].d = ViewProjection[3][3] - ViewProjection[3][1];
    // Bottom clipping plane
    m_planes[3].a = ViewProjection[0][3] + ViewProjection[0][1];
    m_planes[3].b = ViewProjection[1][3] + ViewProjection[1][1];
    m_planes[3].c = ViewProjection[2][3] + ViewProjection[2][1];
    m_planes[3].d = ViewProjection[3][3] + ViewProjection[3][1];
    // Near clipping plane
    m_planes[4].a = ViewProjection[0][2];
    m_planes[4].b = ViewProjection[1][2];
    m_planes[4].c = ViewProjection[2][2];
    m_planes[4].d = ViewProjection[3][2];
    // Far clipping plane
    m_planes[5].a = ViewProjection[0][3] - ViewProjection[0][2];
    m_planes[5].b = ViewProjection[1][3] - ViewProjection[1][2];
    m_planes[5].c = ViewProjection[2][3] - ViewProjection[2][2];
    m_planes[5].d = ViewProjection[3][3] - ViewProjection[3][2];
    
    m_planes[0].NormalizePlane();
    m_planes[1].NormalizePlane();
    m_planes[2].NormalizePlane();
    m_planes[3].NormalizePlane();
    m_planes[4].NormalizePlane();
    m_planes[5].NormalizePlane();
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
Frustum::~Frustum(void)
{
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
bool Frustum::IsInside(const Bbox& boundingBox)
{
    const Vector3 center = boundingBox.center();
    const float radius = boundingBox.radius();

    for (size_t counter = 0; counter < 6; ++counter)
    {
        if (m_planes[counter].ClassifyPoint(center) < -radius)
        {
            return false;
        }
    }

    return true;
}


///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void Frustum::ViewPlane::NormalizePlane()
{
    float mag = sqrt(a * a + b * b + c * c);
    a = a / mag;
    b = b / mag;
    c = c / mag;
    d = d / mag;
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
float Frustum::ViewPlane::ClassifyPoint(const Vector3& pt)
{
    return a * pt.x() + b * pt.y() + c * pt.z() + d;
}

