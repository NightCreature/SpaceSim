#pragma once

#include "EntityTagManager.h"
#include "Gameplay/ECS/Component.h"
#include "Gameplay/ECS/Components/ComponentType.h"
#include "Components/ComponentType.h"

#include <unordered_map>

namespace ECS
{

//Effectively a data handle and indirection object
class Entity
{
public:

    static size_t InvalidEntityId;

    Entity() {}
    Entity(size_t id) : m_entityId(id) {}
    //Has some tags here to indicate which data objects it supports
    void AddComponent(ComponentType* componentType)
    {
        m_attachedComponents |= componentType->GetComponentType();
        m_components[componentType->GetComponentType()] = componentType->GetCreateFP()();

        //Should check if we need to create the data here
    }

    const ComponentTag& GetAttachedComponents() const { return m_attachedComponents; }
    

    template<class Component>
    Component* GetComponentData()
    {
        const ComponentTag& tag = DefaultComponentType<Component>::m_instance->GetComponentType();
        auto iterator = m_components.find(tag);
        //access the data store and retrieve the information for this entity
        if (iterator != m_components.end())
        {
            return static_cast<Component*>(m_components[tag]);
        }

        return nullptr;
    }

    //void AddTag(DataTag* tag);
    //DataStore* m_data;

    bool IsValid() const { return m_entityId != InvalidEntityId; }

    size_t GetId() const { return m_entityId; }
private:
    //This is not awesome since its a list instead of a flat map
    std::unordered_map<ComponentTag, Component*> m_components;
    ComponentTag m_attachedComponents;
    size_t m_entityId = InvalidEntityId;//init to invalid
    
};
}