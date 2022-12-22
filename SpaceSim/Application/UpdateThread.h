#pragma once

#include "Core/MessageSystem/MessageObserver.h"
#include "Core/Thread/Thread.h"
#include "Gameplay/ECS/Systems/RenderableSystem.h"
#include "Input/Input.h"

#include <vector>
class EntityManager;
class CameraManager;
class SettingsManager;
class GameObjectManager;
class LaserManager;
class InputSystem;
class RenderInstance;

namespace MessageSystem
{
class MessageQueue;
}

namespace Physics
{
class PhysicsManager;
}

namespace ECS
{
class SystemsManager;
}

class Resource;

class UpdateThread : public Thread
{
public:

    virtual ~UpdateThread() override;

    void Initialise(Resource* resource);

    virtual int WorkerFunction() override;

    void SetElapsedTime(float elapsedTime, double time) { m_elapsedTime = elapsedTime; m_time = time; }
    void UnblockThread() { m_done = false; }

    std::vector<RenderInstance*> GetRenderInstanceList() { return m_renderList; }

    void LockCriticalSection() { EnterCriticalSection(&m_criticalSection); }
    void UnLockCriticalSection() { LeaveCriticalSection(&m_criticalSection); }

    void setInput(Input input) { m_input = input; }

    std::vector<RenderInstance*> m_renderList;

    EntityManager* m_entityManager = nullptr;
    ECS::SystemsManager* m_entitySystemsManager = nullptr;
    Physics::PhysicsManager* m_physicsManager = nullptr;
    GameObjectManager* m_gameObjectManager = nullptr;
    LaserManager* m_laserManager = nullptr;

    MessageSystem::MessageObserver m_messageObservers;

    Input m_input;

    Resource* m_resource = nullptr;

    double m_time;
    float m_elapsedTime;
    

    bool m_done;
};