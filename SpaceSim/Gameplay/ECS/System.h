#pragma once

class Resource;
class EntityManager;
class Entity;

namespace ECS
{

class Entity;

class System
{
public:
    System() {}
    virtual ~System() {}
    void Initialise(Resource* resource, EntityManager* entityManager)
    {
        m_resource = resource;
        m_entityManager = entityManager;
    }
    virtual void AddEntity(Entity& entity) = 0;
    virtual void PrePhysicsUpdate() {}
    virtual void Update() = 0;
    virtual void PostPhysicsUpdate() {}
protected:
    Resource* m_resource = nullptr;
    EntityManager* m_entityManager = nullptr;
};

}