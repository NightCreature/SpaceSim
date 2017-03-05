#pragma once

#include "Core/StringOperations/StringHelperFunctions.h"

#include <string>

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