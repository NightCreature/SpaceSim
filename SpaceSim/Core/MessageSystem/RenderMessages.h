#pragma once

#include "Core/MessageSystem/Messages.h"

#include "Graphics/Light.h"

namespace MessageSystem
{

class CreateLightMessage : public Message
{
public:
    struct LightMessageData : public MessageData
    {
        Light m_light;
        std::string m_lightName;
    };

    CreateLightMessage() { m_MessageId = MESSAGE_ID(CreateLightMessage); m_implementationData = new LightMessageData(); }

    void SetupLightData(const Light& light, const std::string& str) 
    {
        if (m_implementationData)
        {
            LightMessageData* data = static_cast<LightMessageData*>(m_implementationData);
            data->m_light = light;
            data->m_lightName = str;
        }
    }

    const MessageSystem::CreateLightMessage::LightMessageData* getLightData() const
    {
        return static_cast<LightMessageData*>(m_implementationData);
    }


};

class CreatedRenderResourceMessage : public Message
{
public:
    struct CreatedRenderResource : public MessageData
    {
        size_t m_gameObjectId;
        size_t m_renderResourceHandle;
    };

    CreatedRenderResourceMessage() 
    {
        m_MessageId = MESSAGE_ID(CreatedRenderResourceMessage);
        m_implementationData = new CreatedRenderResource();
    }

    void SetData(const CreatedRenderResource& data) { (*static_cast<CreatedRenderResource*>(m_implementationData)) = data; }
    const CreatedRenderResource* GetData() const { return static_cast<CreatedRenderResource*>(m_implementationData); }
};

}