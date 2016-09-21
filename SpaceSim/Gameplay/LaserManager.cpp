#include "Gameplay/LaserManager.h"
#include "Core/Resource/GameResource.h"
#include "Graphics/DeviceManager.h"
#include "Graphics/EffectCache.h"
#include "Graphics/modelmanager.h"

#include "brofiler.h"

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
LaserManager::LaserManager() : m_enemyLaser(nullptr), m_playerLaser(nullptr), m_currentNumberOfEnemyLasers(0), m_currentNumberOfPlayerLasers(0)
{
    for (RenderInstancePool::iterator it = m_renderInstancePool.begin(); it != m_renderInstancePool.end(); ++it)
    {
        *it = new RenderInstance(nullptr, nullptr);
    }
}


//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
LaserManager::~LaserManager()
{
    if (m_enemyLaser != nullptr)
    {
        delete m_enemyLaser;
    }
    if (m_playerLaser != nullptr)
    {
        delete m_playerLaser;
    }

    if (m_renderInstancePool.size() > 0 )
    {
        for (RenderInstancePool::iterator it = m_renderInstancePool.begin(); it != m_renderInstancePool.end(); ++it)
        {
            RenderInstance* ri = *it;
            delete ri;
        }
    }

    for (RenderInstanceQueue::iterator it = m_renderInstances.begin(); it != m_renderInstances.end(); ++it)
    {
        RenderInstance* ri = *it;
        delete ri;
    }
    m_renderInstances.clear();
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void LaserManager::initialise(Resource* resource)
{
    GameResourceHelper helper(resource);
    ShaderInstance enemyShaderInstance, playerShaderInstance;
    m_enemyLaser = helper.getGameResource().getModelManager().LoadModel(resource, enemyShaderInstance, "Models\\enemy_laser.mml");
    m_playerLaser = helper.getGameResource().getModelManager().LoadModel(resource, playerShaderInstance, "Models\\player_laser.mml");
    //m_geometry->initialise(shaderInstance);
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void LaserManager::addInstance( const Vector3& position, const Vector3& direction, const ShaderInstance& shaderInstance, bool player_laser /*= false*/)
{
    //probably need to rotate the laser to the requested direction currently its standing straight up from 0,0,0 along the y-axis
    if (player_laser)
    {
        if (m_currentNumberOfPlayerLasers < m_maxLasers)
        {
            m_worldPlayerTransforms.push_back(Laser(position, direction, shaderInstance));
            ++m_currentNumberOfPlayerLasers;
        }
    }
    else
    {
        if (m_currentNumberOfEnemyLasers < m_maxLasers)
        {
            m_worldEnemyTransforms.push_back(Laser(position, direction, shaderInstance));
            ++m_currentNumberOfEnemyLasers;
        }
    }
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void LaserManager::update(RenderInstanceTree& renderInstances, float time, const DeviceManager& deviceManager)
{
    BROFILER_CATEGORY("LaserManager::update", Profiler::Color::PaleGreen)

    for (RenderInstanceQueue::iterator it = m_renderInstances.begin(); it != m_renderInstances.end(); ++it)
    {
        RenderInstance* ri = *it;
        delete ri;
    }
    m_renderInstances.clear();

    for (std::vector< Laser>::iterator it = m_worldPlayerTransforms.begin(); it != m_worldPlayerTransforms.end(); ++it)
    {
        it->update(time, deviceManager);
        const LaserInstanceData& lid = it->getInstanceData();
        RenderInstance* ri = new RenderInstance(&(m_playerLaser->getMeshData()[0]->getGeometryInstance()), &(lid.m_shaderInstance));
#ifdef _DEBUG
        ri->m_name = L"Player Laser RI";
#endif
        renderInstances.push_back(ri);
        m_renderInstances.push_back(ri);
    }

    for (std::vector< Laser>::iterator it = m_worldEnemyTransforms.begin(); it != m_worldEnemyTransforms.end(); ++it)
    {
        it->update(time, deviceManager);
        const LaserInstanceData& lid = it->getInstanceData();
        RenderInstance* ri = new RenderInstance(&(m_enemyLaser->getMeshData()[0]->getGeometryInstance()), &(lid.m_shaderInstance));
#ifdef _DEBUG
        ri->m_name = L"Enemy Laser RI";
#endif
        renderInstances.push_back(ri);
        m_renderInstances.push_back(ri);
    }
}
