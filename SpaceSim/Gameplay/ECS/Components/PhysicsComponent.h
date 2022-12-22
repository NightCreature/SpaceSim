#pragma once

#include "Gameplay/ECS/Component.h"

namespace physx
{
    class PxActor;
}

namespace ECS
{

    

class PhysicsComponent : public Component
{
public:
    DECLARE_COMPONENT(PhysicsComponent);
    
    static constexpr size_t InvalidPhysicsId = static_cast<size_t>(-1);

    //Temporary interface probably need this in physics manager and not here
    void CreateRigidbody(bool dynamic);
private:
    physx::PxActor* m_actor = nullptr;
    size_t m_physicsObjectId = InvalidPhysicsId;
};

}