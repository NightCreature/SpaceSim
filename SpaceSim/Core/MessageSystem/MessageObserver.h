#pragma once

#include "MessageQueue.h"
#include "FastDelegate.h"

namespace MessageSystem
{

struct MessageObserverBinding
{
    MessageObserverBinding(const Message::MessageId& type, const fastdelegate::FastDelegate1<const Message&>& dispatchFunction ) : m_type(type), m_dispatchFunction(dispatchFunction) {}

    Message::MessageId m_type;
    fastdelegate::FastDelegate1<const Message&> m_dispatchFunction;
};

class MessageObserver
{
public:
    MessageObserver();
    virtual ~MessageObserver();

    void AddDispatchFunction(const Message::MessageId& type, const fastdelegate::FastDelegate1<const Message&>& dispatchFunction);
    void RemoveDispatchFunction(const fastdelegate::FastDelegate1<const Message&>& dispatchFunction);
    void DispatchMessages(const MessageQueue& messageQueue);
private:
    std::vector<MessageObserverBinding> m_bindings;
};

}