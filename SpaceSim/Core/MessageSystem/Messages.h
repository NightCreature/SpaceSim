#pragma once

#include "Core/StringOperations/StringHelperFunctions.h"

#include <string>

#define MESSAGE_ID(message_name) hashString(#message_name)

namespace MessageSystem
{

class Message;

class IMessageDispatcher
{
public:
    virtual ~IMessageDispatcher() {}
    virtual void dispatchMessage(const Message& msg) = 0;
};

class Message
{
public:
    typedef size_t MessageId;
    MessageId getMessageId() const { return m_MessageId; }
    void* GetImplementationData() const { return m_implementationData; }
    size_t GetImplementationDataSize() const { return m_implementationDataSize; }

    virtual void CleanupImplementationData() {}
protected:
    MessageId m_MessageId;
    void* m_implementationData;
    size_t m_implementationDataSize;
};

}