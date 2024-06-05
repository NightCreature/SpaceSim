#ifndef LASER_H
#define LASER_H

#include "Gameplay/box.h"
#include "Gameplay/bbox.h"
#include "Math/matrix44.h"
#include "Graphics/ShaderInstance.h"
#include "Math/vector3.h"

class DeviceManager;

struct LaserInstanceData
{
    LaserInstanceData() : world() {}
    Matrix44 world;

    Bbox m_boundingBox;
};

class Laser
{
public:
	Laser() {}
	Laser(const Vector3& pos, const Vector3& dir) : m_position(pos), m_direction(dir) {}
	~Laser() {}

    void update(float elapsedTime, const Matrix44& view, const Matrix44& projection);

    const Vector3& getPosition() const { return m_position; }
    const Vector3& getDirection() const { return m_direction; }

    LaserInstanceData& getInstanceData()
    {
        return m_instanceData;
    }
protected:
private:
    void updateInstanceData(const Matrix44& view, const Matrix44& projection);

    LaserInstanceData m_instanceData;
    Vector3 m_position;
    Vector3 m_direction;
};
#endif