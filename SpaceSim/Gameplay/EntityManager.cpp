#include "EntityManager.h"

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
std::vector<ECS::Entity*> EntityManager::GetEntitiesForTag(const ECS::ComponentTag& tag)
{
    std::vector<ECS::Entity*> list;
    for (auto& entity : m_entities)
    {
        if ((entity.GetAttachedComponents() & tag).any()) //really would like to see if this is just bigger than zero
        {
            //Entity belongs to these types of entities
            list.push_back(&entity);
        }
    }

    return list;
}
