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

#include "Brofiler.h"


//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void UpdateThread::Initialise()
{

}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
int UpdateThread::workerFunction()
{
    while (isAlive())
    {
        BROFILER_THREAD("UpdateThread");
        if (!m_done)
        {
            EnterCriticalSection(&m_criticalSection);

            m_messageObservers.DispatchMessages(*m_messageQueue); //Dispatch the messages
            m_messageQueue->reset(); //Reset the queue so we can track new message in it

            m_renderList.clear();

            m_gameObjectManager->update(m_renderList, m_elapsedTime, m_input);
            m_laserManager->update(m_renderList, m_elapsedTime);

            m_done = true;
            LeaveCriticalSection(&m_criticalSection);
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