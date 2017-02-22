#pragma once

#include <array>
#include <vector>

namespace MessageSystem
{

class Message
{
public:
    typedef size_t MessageId;
    MessageId getMessageId() const { return m_MessageId; }
protected:
    MessageId m_MessageId;
};

class MessageQueue
{
public:
    MessageQueue() : m_numberOfMessages(0) {}
    ~MessageQueue() {}

    void addMessage(const Message& msg) { m_messages[m_numberOfMessages] = msg; ++m_numberOfMessages; }
    void reset() { m_numberOfMessages = 0; }

    typedef std::array<Message, 1024> Messages;
    const Messages& getMessages() const { return m_messages; }
    size_t numberOfMessagages() const { return m_numberOfMessages; }
private:
    Messages m_messages;
    size_t m_numberOfMessages;
};
}