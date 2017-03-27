#pragma once

#include "Core/MessageSystem/Messages.h"

#include "Graphics/Light.h"

namespace MessageSystem
{

class CreateLightMessage : public Message
{
public:
    struct LightMessageData
    {
        Light m_light;
        std::string m_lightName;
    };

    CreateLightMessage() { m_MessageId = MESSAGE_ID(CreateLightMessage); m_implementationData = static_cast<void*>(new LightMessageData()); }

    virtual void CleanupImplementationData() { delete static_cast<LightMessageData*>(m_implementationData); }
    void SetupLightData(const Light& light, const std::string& str) 
    {
        LightMessageData* data = static_cast<LightMessageData*>(m_implementationData);
        data->m_light = light;
        data->m_lightName = str;
    }

    const MessageSystem::CreateLightMessage::LightMessageData* getLightData() const
    {
        return static_cast<LightMessageData*>(m_implementationData);
    }


};

class CreatedRenderResourceMessage : public Message
{
public:
    struct CreatedRenderResource
    {
        size_t m_gameObjectId;
        size_t m_renderResourceHandle;
    };

    CreatedRenderResourceMessage() 
    {
        m_MessageId = MESSAGE_ID(CreatedRenderResourceMessage);
        m_implementationData = static_cast<void*>(new CreatedRenderResource());
    }

    virtual void CleanupImplementationData() { delete static_cast<CreatedRenderResource*>(m_implementationData); }

    void SetData(const CreatedRenderResource& data) { (*static_cast<CreatedRenderResource*>(m_implementationData)) = data; }
    const CreatedRenderResource* GetData() const { return static_cast<CreatedRenderResource*>(m_implementationData); }
};

}