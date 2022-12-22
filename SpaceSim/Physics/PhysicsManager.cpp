#include <Physics/PhysicsManager.h>

#include "Core/StringOperations/StringHelperFunctions.h"
#include "Core/Resource/GameResource.h"

#include <map>
#include "Core/Types/TypeHelpers.h"

#include "extensions\PxExtensionsAPI.h"
#include "pvd/PxPvd.h"
#include "pvd/PxPvdTransport.h"

namespace Physics
{

//Probably want to override these specially the error callback
static PhysXAllocatorCallback physxAllocatorCallback;
static PhysXErrorLogCallback physxErrorCallback;

size_t PhysicsManager::m_currentFreeId = 0;

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void PhysicsManager::Initialise(Resource* resource)
{
    m_resource = resource;
    //Init PhysX here

    m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, physxAllocatorCallback, physxErrorCallback);
    if (!m_foundation)
    {
        MSG_TRACE_CHANNEL("PhysicsManager", "Failed to create PhysX foundation");
    }


#ifndef IS_RELEASE
    m_pvd = physx::PxCreatePvd(*m_foundation);
    m_transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
    m_pvd->connect(*m_transport, physx::PxPvdInstrumentationFlag::eALL);

    //if (!PxInitExtensions(*m_physics, m_pvd))
    //{
    //    MSG_TRACE_CHANNEL("PhysicsManager", "!PxInitExtensions(*m_physics, m_pvd)");
    //}
#endif

    bool recordMemoryAllocations = true;

    m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, physx::PxTolerancesScale(), recordMemoryAllocations, m_pvd);
    if (!m_physics)
    {
        MSG_TRACE_CHANNEL("PhysicsManager", "Failed to create PhysX physics object");
    }
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void PhysicsManager::Cleanup()
{
    m_physics->release();
#ifndef IS_RELEASE
    m_pvd->disconnect();

    m_transport->release();

    m_pvd->release();
#endif
    m_foundation->release();
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void PhysicsManager::Update(float elpasedTime)
{
    //This needs to step the scenes we have
    UNUSEDPARAM(elpasedTime);
}

///-----------------------------------------------------------------------------
///! @brief   Creates and adds object to physics world
///! @remark
///-----------------------------------------------------------------------------
void PhysicsManager::CreateAndAddNewObject(size_t& id, const PhysicsProperties& properties)
{
    id = m_currentFreeId;
    ++m_currentFreeId;

    //Create object based on properties here
/////-----------------------------------------------------------------------------
/////! @brief   Walls for example in space sim are PxRigidStatic they dont move and have infinite mass and inertia, interactable objects are PxRigidDynamic objects since they can be woken up to do things
/////! @remark
/////-----------------------------------------------------------------------------
//void ECS::PhysicsComponent::CreateRigidbody(bool dynamic)
//{
//    //Physx uses inheritance to determine if something is static or movable with the PxRigidStatic and PxDynamicRigid classes.
//    UNUSEDPARAM(dynamic);
//
//}

}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void PhysXErrorLogCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
{
    static std::map<physx::PxErrorCode::Enum, TraceSeverity> physxToTraceSeverity =
    {
        { physx::PxErrorCode::eNO_ERROR, TraceSeverity::ELOG },
        { physx::PxErrorCode::eDEBUG_INFO, TraceSeverity::EDEBUG },
        { physx::PxErrorCode::eDEBUG_WARNING, TraceSeverity::EWARN },
        { physx::PxErrorCode::eINVALID_PARAMETER, TraceSeverity::EWARN },
        { physx::PxErrorCode::eINVALID_OPERATION, TraceSeverity::EWARN },
        { physx::PxErrorCode::eOUT_OF_MEMORY, TraceSeverity::EWARN },
        { physx::PxErrorCode::eINTERNAL_ERROR, TraceSeverity::EWARN },
        { physx::PxErrorCode::eABORT, TraceSeverity::EWARN }
    };


    debugOutput(physxToTraceSeverity[code], "PhysX", file, line, "%s", message);
}

}