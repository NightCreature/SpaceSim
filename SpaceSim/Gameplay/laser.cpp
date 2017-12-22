#include "Application/BaseApplication.h"
#include "Gameplay/laser.h"
#include "Graphics/material.h"
#include "Math/matrix44.h"
#include "Graphics/DeviceManager.h"
#include "Core/Types/TypeHelpers.h"
#include <sstream>


///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void Laser::update(float elapsedTime, const Matrix44& view, const Matrix44& projection)
{
    m_position += m_direction * elapsedTime;
    updateInstanceData(view, projection);
}

void Laser::updateInstanceData(const Matrix44& view, const Matrix44& projection)
{
    Vector3 direction = m_direction;
    direction.normalize();
    m_instanceData.world = rotationFromDirection(direction) * translate(m_position);
    //WVPBufferContent& wvpConstants = m_instanceData.m_shaderInstance.getWVPConstants();
    //wvpConstants.m_projection = projection;
    //wvpConstants.m_view = view;
    //wvpConstants.m_world = m_instanceData.world;
    //m_instanceData.m_shaderInstance.getMaterial().getEffect()->setWVPContent(deviceManager, wvpConstants);
    UNUSEDPARAM(view);
    UNUSEDPARAM(projection);
}
