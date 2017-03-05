#pragma once

#include "Core/MessageSystem/MessageObserver.h"
#include "Core/Thread/Thread.h"
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

class UpdateThread : public Thread
{
public:

    void Initialise();

    virtual int workerFunction() override;

    void SetElapsedTime(float elapsedTime, double time) { m_elapsedTime = elapsedTime; m_time = time; }
    void UnblockThread() { m_done = false; }

    std::vector<RenderInstance*> GetRenderInstanceList() { return m_renderList; }

    void LockCriticalSection() { EnterCriticalSection(&m_criticalSection); }
    void UnLockCriticalSection() { LeaveCriticalSection(&m_criticalSection); }

    void setInput(Input input) { m_input = input; }
    std::vector<RenderInstance*> m_renderList;

    EntityManager* m_entityManager;
    GameObjectManager* m_gameObjectManager;
    LaserManager* m_laserManager;

    MessageSystem::MessageObserver m_messageObservers;

    Input m_input;

    double m_time;
    float m_elapsedTime;
    

    bool m_done;
};