#include "Graphics/Frustum.h"

#include "Gameplay/bbox.h"

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
Frustum::Frustum( const Matrix44& viewMatrix, const Matrix44& projectionMatrix)
{

    Matrix44 ViewProjection = viewMatrix * projectionMatrix;

    m_planes[0].m_normal = Vector3( ViewProjection[0][2] - ViewProjection[0][0],
                                      ViewProjection[1][3] - ViewProjection[1][0],
                                      ViewProjection[2][3] - ViewProjection[2][0]);
    m_planes[0].m_distanceFromOrigin = ViewProjection[3][3] - ViewProjection[3][0];
    m_planes[0].normalize();

    m_planes[1].m_normal = Vector3( ViewProjection[0][3] + ViewProjection[0][0],
                                      ViewProjection[1][3] + ViewProjection[1][0],
                                      ViewProjection[2][3] + ViewProjection[2][0]);
    m_planes[1].m_distanceFromOrigin = ViewProjection[3][3] + ViewProjection[3][0];
    m_planes[1].normalize();

    m_planes[2].m_normal = Vector3( ViewProjection[0][3] - ViewProjection[0][1],
                                      ViewProjection[1][3] - ViewProjection[1][1],
                                      ViewProjection[2][3] - ViewProjection[2][1]);
    m_planes[2].m_distanceFromOrigin = ViewProjection[3][3] - ViewProjection[3][1];
    m_planes[2].normalize();

    m_planes[3].m_normal = Vector3( ViewProjection[0][3]+ViewProjection[0][1],
                                      ViewProjection[1][3]+ViewProjection[1][1],
                                      ViewProjection[2][3]+ViewProjection[2][1]);
    m_planes[3].m_distanceFromOrigin = ViewProjection[3][3]+ViewProjection[3][1];
    m_planes[3].normalize();

    m_planes[4].m_normal = Vector3( ViewProjection[0][3] - ViewProjection[0][2],
                                      ViewProjection[1][3] - ViewProjection[1][2],
                                      ViewProjection[2][3] - ViewProjection[2][2]);
    m_planes[4].m_distanceFromOrigin = ViewProjection[3][3] - ViewProjection[3][2];
    m_planes[4].normalize();

    m_planes[5].m_normal = Vector3( ViewProjection[0][3] + ViewProjection[0][2],
                                      ViewProjection[1][3] + ViewProjection[1][2],
                                      ViewProjection[2][3] + ViewProjection[2][2]);
    m_planes[5].m_distanceFromOrigin = ViewProjection[3][3] + ViewProjection[3][2];
    m_planes[5].normalize();
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
Frustum::~Frustum(void)
{
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
bool Frustum::IsInside(const Bbox& boundingBox)
{
    const Vector3 center = boundingBox.center();
    const Vector3 raduis = boundingBox.radius();

    return true;
}


