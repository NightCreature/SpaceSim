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
    void addInstance( const Vector3& position, const Vector3& direction, const ShaderInstance& shaderInstance, bool player_laser = false);

    void update(RenderInstanceTree& renderInstances, float time, const Matrix44& view, const Matrix44& projection);

    typedef std::vector<RenderInstance* > RenderInstancePool;
    typedef std::vector< RenderInstance* > RenderInstanceQueue;
    typedef std::pair< Laser, RenderInstance*> LasterRenderPair;
    typedef std::vector< LasterRenderPair > LaserQueue; //This should be a map or a list of pair from laser to trenderinstance so that we dont recreate everything every frame
private:
    LaserQueue m_worldEnemyTransforms;
    LaserQueue m_worldPlayerTransforms;
    RenderInstanceQueue m_renderInstances;
    RenderInstancePool m_renderInstancePool;
    Model* m_enemyLaser;
    Model* m_playerLaser;

    const static size_t m_maxLasers = 1000;
    size_t m_currentNumberOfEnemyLasers;
    size_t m_currentNumberOfPlayerLasers;
};
