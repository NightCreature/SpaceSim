#pragma once

#include "Core/Macros/PlatformMacros.h"

#ifndef IS_CLANG //Physx On windows has no Clang compile at the moment
#include "PxConfig.h"
#include "PxPhysicsAPI.h"
#endif

#include <crtdbg.h>
#include <vector>

class Resource;
namespace Physics
{

class PhysicsObject;
class PhysicsProperties;

#ifndef IS_CLANG
class PhysXErrorLogCallback : public physx::PxErrorCallback
{

public:

    void reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line) override;

};

class PhysXAllocatorCallback : public physx::PxAllocatorCallback
{
public:
    void* allocate(size_t size, const char*, const char*, int)
    {
        void* ptr = _aligned_malloc(size, 16);
        //PX_ASSERT((reinterpret_cast<size_t>(ptr) & 15) == 0);
        return ptr;
    }

    void deallocate(void* ptr)
    {
        _aligned_free(ptr);
    }
};
#endif

class PhysicsManager
{
public:
    PhysicsManager() {}
    ~PhysicsManager() {}

    void Initialise(Resource* resource);
    void Cleanup();

    void Update(float elpasedTime);

    void CreateAndAddNewObject(size_t& id, const PhysicsProperties& properties);
private:
    Resource* m_resource = nullptr;

#ifndef IS_CLANG
    physx::PxFoundation* m_foundation = nullptr;
    physx::PxPhysics* m_physics = nullptr;
    physx::PxPvd* m_pvd = nullptr;
#ifndef IS_RELEASE
    physx::PxPvdTransport* m_transport = nullptr;
#endif
#endif //IS_CLANG

    std::vector<PhysicsObject*> m_worldObjects;//should probably not be a pointer but for now this is ok
    static size_t m_currentFreeId;
};

}