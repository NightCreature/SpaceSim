#pragma once

#include "Core/MessageSystem/Messages.h"

namespace MessageSystem
{

class CreateRenderResourceMessage : public Message
{
private:
    size_t m_gameObjectHandle;
};

class CreatePlaneMessage : public CreateRenderResourceMessage
{
public:
    CreatePlaneMessage(const Face::CreationParams& planeData) 
    { 
        m_MessageId = MESSAGE_ID(CreatePlaneMessage);
        m_implementationData = static_cast<void*>(new Face::CreationParams()); 
        memcpy(&m_implementationData, &planeData, sizeof(Face::CreationParams));
    }
};

}