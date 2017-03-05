#pragma once

#include "Core/StringOperations/StringHelperFunctions.h"

#include <string>

#define MESSAGE_ID(message_name) hashString(#message_name)

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

}