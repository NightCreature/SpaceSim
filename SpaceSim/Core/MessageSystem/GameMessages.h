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
    CreatePlaneMessage() 
    { 
        m_MessageId = MESSAGE_ID(CreatePlaneMessage);
        m_implementationData = static_cast<void*>(new Face::CreationParams()); 
    }

    void SetData(const Face::CreationParams& params)
    {
        Face::CreationParams* implementatioParams = static_cast<Face::CreationParams*>(m_implementationData);
        (*implementatioParams) = params;
    }
};

}