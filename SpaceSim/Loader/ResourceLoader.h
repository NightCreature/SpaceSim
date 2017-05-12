#pragma once

#include "Core/MessageSystem/Messages.h"

#include <vector>

class Resource;

class ResourceLoader : public MessageSystem::IMessageDispatcher
{
public:

    struct LoadRequest
    {
        size_t m_resourceType;
        size_t m_gameObjectId;
        void* m_loadData;
    };

    ResourceLoader() {}
    ~ResourceLoader() {}

    void initialise(Resource* resource) { m_resource = resource; }
    void update();

    virtual void dispatchMessage(const MessageSystem::Message & msg) override;
    void AddLoadRequest(const LoadRequest& request);
public:

    std::vector<LoadRequest> m_requests;
    std::vector<LoadRequest> m_newRequestsDuringUpdate;
    bool m_isUpdating;
    Resource* m_resource;
    

};