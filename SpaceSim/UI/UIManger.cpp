#include "UIManger.h"

#include "UI/Core/Behaviours/behaviours.h"
#include "UI/Core/States/States.h"

//
//#include "StringHelperFunctions.h"
//
/////-----------------------------------------------------------------------------
/////! @brief   TODO enter a description
/////! @remark
/////-----------------------------------------------------------------------------
//void UIManger::initialise()
//{
//    unsigned long threadId = 0;
//    size_t stackSize = 2048u * 1024u * 1024u;
//    m_threadHandle = CreateThread(nullptr, stackSize, &WebkitThread::ThreadEntryFunction, &m_webKitThread, 0, &threadId);
//
//    if (!m_threadHandle)
//    {
//        MSG_TRACE_CHANNEL("UIManager", "Failed to create webkit thread");
//    }
//}

void UIManger::Initialise(Resource* resource)
{
    m_resource = resource;

    FE::States::RegisterStates();
    FE::Behaviours::RegisterBehaviours();
    //FE::Transitions::RegisterTransitions();

    m_stateManager.Initialise(m_resource);
    m_flowManager.Initialise(m_resource, &m_stateManager);

    m_stateManager.Serialise("States/states.xml");
    m_flowManager.Serialise("Flow/flow.xml");

    m_flowManager.ActivateFlow();
}

void UIManger::Update(float deltaTime, const InputState& input)
{
    m_flowManager.Update(deltaTime, input);
}
