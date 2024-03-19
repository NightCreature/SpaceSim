#include "PhysicsComponent.h"

#include "PxActor.h"
#include "Core/Types/TypeHelpers.h"

///-----------------------------------------------------------------------------
///! @brief   Walls for example in space sim are PxRigidStatic they dont move and have infinite mass and inertia, interactable objects are PxRigidDynamic objects since they can be woken up to do things
///! @remark
///-----------------------------------------------------------------------------
void ECS::PhysicsComponent::CreateRigidbody(bool dynamic)
{
    //Physx uses inheritance to determine if something is static or movable with the PxRigidStatic and PxDynamicRigid classes.
    UNUSEDPARAM(dynamic);
    m_physicsObjectId = 1;
    
}
