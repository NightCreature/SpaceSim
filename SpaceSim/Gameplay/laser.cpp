#include "Application/BaseApplication.h"
#include "Gameplay/laser.h"
#include "Graphics/material.h"
#include "Math/matrix44.h"
#include "Graphics/DeviceManager.h"
#include "Core/Types/TypeHelpers.h"
#include <sstream>


//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void Laser::update(float elapsedTime)
{
    m_position += m_direction * elapsedTime;
    updateInstanceData();
}

void Laser::updateInstanceData()
{
    Vector3 direction = m_direction;
    direction.normalize();
    m_instanceData.world = rotationFromDirection(direction) * translate(m_position);
    WVPBufferContent& wvpConstants = m_instanceData.m_shaderInstance.getWVPConstants();
    wvpConstants.m_projection = Application::m_projection;
    wvpConstants.m_view = Application::m_view;
    wvpConstants.m_world = m_instanceData.world;
    //m_instanceData.m_shaderInstance.getMaterial().getEffect()->setWVPContent(deviceManager, wvpConstants);
}
