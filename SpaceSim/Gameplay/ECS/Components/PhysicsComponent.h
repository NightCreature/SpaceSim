#pragma once

#include "Core/Macros/PlatformMacros.h"
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
#ifndef IS_CLANG
    physx::PxActor* m_actor = nullptr;
#endif
    size_t m_physicsObjectId = InvalidPhysicsId;
};

}