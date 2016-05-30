#pragma once

#include "Graphics/GeometryInstance.h"
#include "Gameplay/Laser.h"
#include "Graphics/VertexBuffer.h"
#include <deque>

class ShaderInstance;
class Vector3;
class Color;
class Resource;
class RenderInstance;
class DeviceManager;

class LaserManager
{
public:
    LaserManager();
    ~LaserManager();

    void initialise(Resource* resource);
    void addInstance( const Vector3& position, const Vector3& direction, const ShaderInstance& shaderInstance);

    void update(RenderInstanceTree& renderInstances, float time, const DeviceManager& deviceManager);

    typedef std::vector<RenderInstance* > RenderInstancePool;
    typedef std::deque< RenderInstance* > RenderInstanceQueue;
    typedef std::deque< Laser > LaserQueue; //This should be a map or a list of pair from laser to trenderinstance so that we dont recreate everything every frame
private:
    LaserQueue m_worldTransforms;
    RenderInstanceQueue m_renderInstances;
    RenderInstancePool m_renderInstancePool;
    Model* m_geometry;

    const static size_t m_maxLasers = 1000;
    size_t m_currentNumberOfLasers;
};
