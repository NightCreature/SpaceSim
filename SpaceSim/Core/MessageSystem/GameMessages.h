#pragma once

#include "Core/MessageSystem/Messages.h"

#include "Math/matrix44.h"

namespace MessageSystem
{

//This has to be inside of the implementation details
class LoadResourceRequest : public Message
{
public:
    struct LoadResourceData : public MessageData
    {
        size_t m_gameObjectHandle;
        size_t m_resourceType;
    };

    LoadResourceRequest() { m_MessageId = MESSAGE_ID(LoadResourceRequest); }

    size_t GetResourceType() const { return static_cast<LoadResourceData*>(m_implementationData)->m_resourceType; }
    size_t GetGameObjectId() const { return static_cast<LoadResourceData*>(m_implementationData)->m_gameObjectHandle; }
    void SetGameObjectId(const size_t gameObjectId) { auto requestData = static_cast<LoadResourceData*>(m_implementationData); requestData->m_gameObjectHandle = gameObjectId; }
protected:

};

template<class T>
class CreateFixedModelResource : public LoadResourceRequest
{
public:
    template<class T>
    struct FixedModelResourceData : public LoadResourceRequest::LoadResourceData
    {
        T m_fixedData;
    };

    CreateFixedModelResource(const char* typeName)
    {
        m_implementationData = new FixedModelResourceData<T>();
        static_cast<FixedModelResourceData<T>*>(m_implementationData)->m_resourceType = hashString(typeName);
        m_implementationDataSize = sizeof(FixedModelResourceData<T>);

        UNUSEDPARAM(typeName); //work arround for compile error parameter is used
    }

    virtual void CleanupImplementationData() { delete static_cast<FixedModelResourceData<T>*>(m_implementationData); }

    void SetData(const T& data) { FixedModelResourceData<T>* dataPtr = static_cast<FixedModelResourceData<T>*>(m_implementationData); (*dataPtr).m_fixedData = data; }
    const FixedModelResourceData<T>* GetData() const { return static_cast<FixedModelResourceData<T>*>(m_implementationData); }
}; 

class RenderInformation : public Message
{
public:
    struct RenderInfo : public MessageData
    {
        Matrix44 m_world;
        size_t m_renderObjectid;
        size_t m_gameobjectid;        
        std::string m_name;
    };

    RenderInformation()
    {
        m_MessageId = MESSAGE_ID(RenderInformation);
        m_implementationData = new RenderInfo();
        m_implementationDataSize = sizeof(RenderInfo);
    }

    virtual void CleanupImplementationData() { delete static_cast<RenderInfo*>(m_implementationData); }

    const RenderInfo* GetData() const { return static_cast<RenderInfo*>(m_implementationData); }
    void SetData(const RenderInfo& data) { (*static_cast<RenderInfo*>(m_implementationData)) = data; }
};

}

#define CREATEFIXEDMODELRESOURCEMESSAGE(type) MessageSystem::CreateFixedModelResource<type>(#type);