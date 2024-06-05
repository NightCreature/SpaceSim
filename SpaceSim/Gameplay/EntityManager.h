#pragma once

#include <assert.h>
#include <deque>
#include <functional>
#include <vector>
#include "ECS/Components/ComponentType.h"
#include "ECS/Entity.h"
#include "ECS/SystemsManager.h"

class EntityManager
{
public:
//
//    //Need to pool create the free indices
    EntityManager()
    {
    }

    ECS::Entity Create() 
    {
        m_entities.emplace_back(ECS::Entity(m_entityIndex++));
        return *(m_entities.rbegin());
    }

    std::vector<ECS::Entity*> GetEntitiesForTag(const ECS::ComponentTag& tag);

    const ECS::SystemsManager& GetSystemsManager() const { return m_systemsManager; }
    ECS::SystemsManager& GetSystemsManager() { return m_systemsManager; }
private:
    ECS::SystemsManager m_systemsManager;
    std::vector<ECS::Entity> m_entities;

    size_t m_entityIndex = 0; //revisit this when it becomes a problem since we only ever increment
};
//
