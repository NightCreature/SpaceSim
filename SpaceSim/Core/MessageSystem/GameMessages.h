#pragma once

#include "Core/MessageSystem/Messages.h"

namespace MessageSystem
{

//This has to be inside of the implementation details
class LoadResourceRequest : public Message
{
public:
    struct LoadResourceData
    {
        size_t m_gameObjectHandle;
        size_t m_resourceType;
    };

    LoadResourceRequest() { m_MessageId = MESSAGE_ID(LoadResourceRequest); }

    size_t GetResourceType() const { return static_cast<LoadResourceData*>(m_implementationData)->m_resourceType; }
    size_t GetGameObjectId() const { return static_cast<LoadResourceData*>(m_implementationData)->m_gameObjectHandle; }
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
        m_implementationData = static_cast<void*>(new FixedModelResourceData<T>());
        static_cast<FixedModelResourceData<T>*>(m_implementationData)->m_resourceType = hashString(typeName);
        m_implementationDataSize = sizeof(FixedModelResourceData<T>);

        UNUSEDPARAM(typeName); //work arround for compile error parameter is used
    }

    void SetData(const T& data) { FixedModelResourceData<T>* dataPtr = static_cast<FixedModelResourceData<T>*>(m_implementationData); (*dataPtr).m_fixedData = data; }
    const FixedModelResourceData<T>* GetData() const { return static_cast<FixedModelResourceData<T>*>(m_implementationData); }
}; 

}

#define CREATEFIXEDMODELRESOURCEMESSAGE(type) MessageSystem::CreateFixedModelResource<type>(#type);