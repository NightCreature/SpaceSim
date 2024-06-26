#pragma once

#include "Core/StringOperations/HashString.h"
#include "Core/StringOperations/StringHelperFunctions.h"

#include <string>

#ifdef _DEBUG
#include "SourceInformation/SourceInfo.h"
#endif


#define MESSAGE_ID(message_name) Hashing::hashString(#message_name)

namespace MessageSystem
{

class Message;

class IMessageDispatcher
{
public:
    virtual ~IMessageDispatcher() {}
    virtual void dispatchMessage(const Message& msg) = 0;
};

class MessageData
{
public:
    MessageData() {}
    virtual ~MessageData() {}
};

class HashStringData : public MessageData
{
public:
    HashStringData() {}
    HashStringData(const HashString& data) : m_data(data) {}

    const HashString& GetData() const { return m_data; }
    void SetData(const HashString& data) { m_data = data; }
private:
    HashString m_data;
};

class Message
{
public:
    virtual ~Message() {}
    typedef size_t MessageId;
    MessageId getMessageId() const { return m_MessageId; }
    void* GetImplementationData() const { return m_implementationData; }
    size_t GetImplementationDataSize() const { return m_implementationDataSize; }

    void CleanupImplementationData() 
    {
        delete m_implementationData; 
        m_implementationData = nullptr; 
        m_implementationDataSize = 0; 
    }
protected:
    MessageId m_MessageId;
    MessageData* m_implementationData;
    size_t m_implementationDataSize;

#ifdef _DEBUG
public:
    SourceInfo m_sourceInfo;
#endif
};

}