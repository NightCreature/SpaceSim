#include "laser.h"
#include "material.h"
#include "matrix44.h"
#include "..\SpaceSim\BaseApplication.h"
#include "..\SpaceSim\DeviceManager.h"
#include "..\SpaceSim\TypeHelpers.h"
#include <sstream>


//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void Laser::update(float elapsedTime, const DeviceManager& deviceManager)
{
    m_position += m_direction * elapsedTime;
    updateInstanceData(deviceManager);
}

void Laser::updateInstanceData(const DeviceManager& deviceManager)
{
    Vector3 direction = m_direction;
    direction.normalize();
    m_instanceData.world = rotationFromDirection(direction) * translate(m_position);
    WVPBufferContent& wvpConstants = m_instanceData.m_shaderInstance.getWVPConstants();
    wvpConstants.m_projection = Application::m_projection;
    wvpConstants.m_view = Application::m_view;
    wvpConstants.m_world = m_instanceData.world;
    //m_instanceData.m_shaderInstance.getMaterial().getEffect()->setWVPContent(deviceManager, wvpConstants);
    UNUSEDPARAM(deviceManager);
}
