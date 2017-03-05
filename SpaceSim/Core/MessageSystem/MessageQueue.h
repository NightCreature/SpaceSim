#pragma once

#include "Core/MessageSystem/Messages.h"

#include <array>
#include <vector>

namespace MessageSystem
{

class MessageQueue
{
public:
    MessageQueue() : m_numberOfMessages(0) {}
    ~MessageQueue() {}

    void addMessage(const Message& msg) { m_messages[m_numberOfMessages] = msg; ++m_numberOfMessages; }
    void reset() { m_numberOfMessages = 0; }

    typedef std::array<Message, 1024> Messages; //This cant be a straight up message because we will lose the internal data of the message
    const Messages& getMessages() const { return m_messages; }
    size_t numberOfMessagages() const { return m_numberOfMessages; }
private:
    Messages m_messages;
    size_t m_numberOfMessages;
};

class MessageQueues
{
public:
    MessageQueues() 
    {
        m_updateQueue = &m_messageQueue[0];
        m_renderQueue = &m_messageQueue[1];
    }
    ~MessageQueues() {}

    MessageQueue* getRenderMessageQueue() { return m_renderQueue; }
    MessageQueue* getUpdateMessageQueue() { return m_updateQueue; }

    void swapQueues()
    {
        MessageSystem::MessageQueue* temp = m_renderQueue;
        m_renderQueue = m_updateQueue;
        m_updateQueue = temp;
    }


private:
    MessageQueue m_messageQueue[2]; //Pass pointers for current and previous to other systems, just swap the render one to update and reset and pass update to render
    MessageQueue* m_updateQueue;
    MessageQueue* m_renderQueue;
};
}