#include "UpdateThread.h"

#include "Gameplay/GameObjectManager.h"
#include "Core/Settings/SettingsManager.h"
#include "Core/Types/Types.h"
#include "Core/Resource/GameResource.h"
#include "Gameplay/LaserManager.h"
#include "Graphics/CameraManager.h"
#include "Input/InputSystem.h"
#include "Core/StringOperations/HashString.h"
#include "Core/MessageSystem/MessageQueue.h"

#include "Core/Resource/GameResource.h"

#include "Core/Profiler/ProfilerMacros.h"

#include "Physics/PhysicsManager.h"
#include "Gameplay/ECS/SystemsManager.h"


///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
UpdateThread::~UpdateThread()
{
    
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void UpdateThread::Initialise(Resource* resource)
{
    m_resource = resource;
    m_messageObservers.AddDispatchFunction(MESSAGE_ID(CreatedRenderResourceMessage), fastdelegate::MakeDelegate(m_gameObjectManager, &GameObjectManager::handleMessage));
    //m_messageObservers.AddDispatchFunction(MESSAGE_ID(CreatedRenderResourceMessage), fastdelegate::MakeDelegate(&m_renderableSystem, &ECS::RenderableSystem::HandleMessage));

    m_uiManager.Initialise(m_resource);
    
    m_done = true;
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
int UpdateThread::WorkerFunction()
{
    while (isAlive())
    {
        //PROFILE_THREAD("UpdateThread");
        
        if (!m_done)
        {
            EnterCriticalSection(&m_criticalSection);

            m_messageObservers.DispatchMessages(*(m_resource->m_messageQueues->getUpdateMessageQueue())); //Dispatch the messages
            m_resource->m_messageQueues->getUpdateMessageQueue()->reset();//m_messageQueue->reset(); //Reset the queue so we can track new message in it

            LeaveCriticalSection(&m_criticalSection);

            m_gameObjectManager->update( m_elapsedTime, m_input);
            
            auto& entitySystemsManager = m_entityManager->GetSystemsManager();
            {
                //This group could be on its own job
                entitySystemsManager.PrePhysicsUpdate();
                m_physicsManager->Update(m_elapsedTime);
                entitySystemsManager.PostPhysicsUpdate();
            }
            //Potentially own job
            entitySystemsManager.Update(); //This one is system that dont have a link with the physics, this could be farmed off to a job
            m_laserManager->update( m_elapsedTime, Matrix44(), Matrix44()); //TODO FIX LASERS

            const InputState* inputState = m_input.getInput(0);
            if (inputState != nullptr)
            {
                m_uiManager.Update(m_elapsedTime, *inputState);
            }

            m_done = true;
            
        }
        else
        {
            //busy spin for now should just yield here
            //yield();
            SwitchToThread();
        }
    }

    return 0;
}