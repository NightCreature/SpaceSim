#include "Gameplay/LaserManager.h"
#include "Core/Resource/GameResource.h"
#include "Graphics/DeviceManager.h"
#include "Graphics/EffectCache.h"

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
LaserManager::LaserManager() : m_geometry(nullptr), m_currentNumberOfLasers(0)
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
    if (m_geometry != nullptr)
    {
        delete m_geometry;
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
    GameResourceHelper helper( resource );
    const Effect* effect = helper.getWritableGameResource().getEffectCache().createEffect(resource, "Shaders\\Effects\\laser_effect.xml");

    ShaderInstance shaderInstance;
    shaderInstance.setMaterial(Material(0.0f, Color::black(), Color::black(), Color::red(), Color::red()));
    shaderInstance.getMaterial().setEffect(effect);

    Box::CreationParams params;
    params.m_dynamic = false;
    params.m_gentexcoords = false;
    params.resource = resource;
    params.shaderInstance = &shaderInstance;
    params.m_lowerleft = Vector3(-0.5f, -0.5f, -0.5f);
    params.m_upperright = Vector3(0.5f, 0.5f, 0.5f);
    m_geometry = Box::CreateBox(params).model;

    //m_geometry->initialise(shaderInstance);
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void LaserManager::addInstance( const Vector3& position, const Vector3& direction, const ShaderInstance& shaderInstance )
{
    if (m_currentNumberOfLasers < m_maxLasers)
    {
        m_worldTransforms.push_back(Laser(position, direction, shaderInstance));
        ++m_currentNumberOfLasers;
    }
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void LaserManager::update(RenderInstanceTree& renderInstances, float time, const DeviceManager& deviceManager)
{
    for (RenderInstanceQueue::iterator it = m_renderInstances.begin(); it != m_renderInstances.end(); ++it)
    {
        RenderInstance* ri = *it;
        delete ri;
    }
    m_renderInstances.clear();

    for (std::deque< Laser>::iterator it = m_worldTransforms.begin(); it != m_worldTransforms.end(); ++it)
    {
        it->update(time, deviceManager);
        const LaserInstanceData& lid = it->getInstanceData();
        RenderInstance* ri = new RenderInstance(&(m_geometry->getMeshData()[0]->getGeometryInstance()), &(lid.m_shaderInstance));
#ifdef _DEBUG
        ri->m_name = L"Laser RI";
#endif
        renderInstances.push_back(ri);
        m_renderInstances.push_back(ri);
    }
}
