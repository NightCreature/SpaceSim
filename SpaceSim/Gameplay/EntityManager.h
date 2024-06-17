#pragma once

#include "Core/Types/TypeHelpers.h"
#include "ECS/Components/ComponentType.h"
#include "ECS/Entity.h"
#include "ECS/SystemsManager.h"

#include <assert.h>
#include <deque>
#include <functional>
#include <optional>
#include <vector>

class EntityManager
{
public:
//
//    //Need to pool create the free indices
    EntityManager()
    {
    }

    ECS::Entity& Create() 
    {
        m_entities.emplace_back(ECS::Entity(m_entityIndex++));
        return *(m_entities.rbegin());
    }

    //This is the interface the Systems use to get at the entities
    std::vector<ECS::Entity*> GetEntitiesForTag(const ECS::ComponentTag& tag);

    //This is the level interface to get at the entities
    bool GetEntity(size_t entityId, ECS::Entity & entityOut) const
    {
        UNUSEDPARAM(entityId);
        UNUSEDPARAM(entityOut);
        //for (ECS::Entity& entity : m_entities)
        //{
        //    if (entity.GetId() == entityId)
        //    {
        //        entityOut = entity;
        //        return true;
        //    }
        //}

        return false;
    }

    const ECS::SystemsManager& GetSystemsManager() const { return m_systemsManager; }
    ECS::SystemsManager& GetSystemsManager() { return m_systemsManager; }
private:
    ECS::SystemsManager m_systemsManager;
    std::vector<ECS::Entity> m_entities;

    size_t m_entityIndex = 0; //revisit this when it becomes a problem since we only ever increment
};
//
