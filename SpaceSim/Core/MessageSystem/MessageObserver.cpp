#include "Core\MessageSystem\MessageObserver.h"

#include <Brofiler.h>

namespace MessageSystem
{

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
MessageObserver::MessageObserver()
{
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
MessageObserver::~MessageObserver()
{
}

//-----------------------------------------------------------------------------
//! @brief   Add an observer by signature
//! @remark  This likely needs more work as its too simple and multiple observers could have the same signature which causes problems
//-----------------------------------------------------------------------------
void MessageObserver::AddDispatchFunction(const Message::MessageId& type, const fastdelegate::FastDelegate1<const Message&>& dispatchFunction)
{
    MessageObserverBinding binding = { type, dispatchFunction };
    m_bindings.push_back(binding);
}

//-----------------------------------------------------------------------------
//! @brief   Remove an observer based on its signature
//! @remark  This likely needs more work as its too simple and multiple observers could have the same signature which causes problems
//-----------------------------------------------------------------------------
void MessageObserver::RemoveDispatchFunction(const fastdelegate::FastDelegate1<const Message&>& dispatchFunction)
{
    std::remove_if(begin(m_bindings), end(m_bindings), [dispatchFunction](const auto& binding) { return binding.m_dispatchFunction == dispatchFunction; });
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void MessageObserver::DispatchMessages(const MessageQueue& messageQueue)
{
    BROFILER_CATEGORY("DispatchMessages", Profiler::Color::Gray);
    const MessageQueue::Messages& messages = messageQueue.getMessages();
    for (size_t counter = 0; counter < messageQueue.numberOfMessagages(); ++counter)
    {
        for (auto binding : m_bindings)
        {
            if (binding.m_type == 0 || binding.m_type == messages[counter].getMessageId())
            {
                binding.m_dispatchFunction(messages[counter]);
            }
        }
    }
}

}
