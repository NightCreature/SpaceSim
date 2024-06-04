#include "Gameplay/ECS/SystemsManager.h"

#include "Gameplay/ECS/System.h"

namespace ECS
{

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void SystemsManager::RegisterSystem(System* system, const UpdatePolicy& policy)
{
    auto insertSystemLambda = [&systems = m_systems](System* system, const UpdatePolicy& policy)
    {
        if (std::find(cbegin(systems[policy]), cend(systems[policy]), system) == cend(systems[policy]))
        {
            systems[policy].push_back(system);
        }
    };

    if ((policy & UpdatePolicy::PrePhysics) == 1)
    {
        insertSystemLambda(system, UpdatePolicy::PrePhysics);
    }

    if ((policy & UpdatePolicy::PostPhysics) == 1)
    {
        insertSystemLambda(system, UpdatePolicy::PostPhysics);
    }

    if ((policy & UpdatePolicy::Update) == 1)
    {
        insertSystemLambda(system, UpdatePolicy::Update);
    }
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void SystemsManager::PrePhysicsUpdate()
{
    if (!m_systems.empty())
    {
        for (auto* system : m_systems[UpdatePolicy::PrePhysics])
        {
            system->PrePhysicsUpdate();
        }
    }
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark This could be running on a job system there is no guarantee that physics has run
///-----------------------------------------------------------------------------
void SystemsManager::Update()
{
    if (!m_systems.empty())
    {
        for (auto* system : m_systems[UpdatePolicy::Update])
        {
            system->PrePhysicsUpdate();
        }
    }

}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void SystemsManager::PostPhysicsUpdate()
{
    if (!m_systems.empty())
    {
        for (auto* system : m_systems[UpdatePolicy::PostPhysics])
        {
            system->PrePhysicsUpdate();
        }
    }
}

}