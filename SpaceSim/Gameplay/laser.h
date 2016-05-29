#ifndef LASER_H
#define LASER_H

#include "box.h"
#include "matrix44.h"
#include "..\SpaceSim\ShaderInstance.h"
#include "vector3.h"

class DeviceManager;

struct LaserInstanceData
{
    LaserInstanceData() : world() {}
    LaserInstanceData(const ShaderInstance& shaderInstance) : m_shaderInstance(shaderInstance), world() {}
    ShaderInstance m_shaderInstance;
    Matrix44 world;
};

class Laser
{
public:
	Laser() {}
	Laser(const Vector3& pos, const Vector3& dir, const ShaderInstance& shaderInstance) : m_position(pos), m_direction(dir), m_instanceData(shaderInstance) {}
	~Laser() {}

    void update(float elapsedTime, const DeviceManager& deviceManager);

    const Vector3& getPosition() const { return m_position; }
    const Vector3& getDirection() const { return m_direction; }

    const LaserInstanceData& getInstanceData() const 
    {
        return m_instanceData;
    }
protected:
private:
    void updateInstanceData(const DeviceManager& deviceManager);

    LaserInstanceData m_instanceData;
    Vector3 m_position;
    Vector3 m_direction;
};
#endif