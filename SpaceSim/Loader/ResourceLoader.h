#pragma once

#include "Core/MessageSystem/Messages.h"

#include <vector>

class Resource;

class ResourceLoader : public MessageSystem::IMessageDispatcher
{
public:
    ResourceLoader() {}
    ~ResourceLoader() {}

    void initialise(Resource* resource) { m_resource = resource; }
    void update();

    virtual void dispatchMessage(const MessageSystem::Message & msg) override;
public:
    struct LoadRequest
    {
        ~LoadRequest() { delete[] m_loadData; }
        size_t m_resourceType;
        size_t m_gameObjectId;
        void* m_loadData;
    };

    std::vector<LoadRequest> m_requests;
    Resource* m_resource;
    

};