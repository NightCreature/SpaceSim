#pragma once

#include "Core/MessageSystem/Messages.h"
#include "Core/Thread/Job.h"

#include <vector>

class Resource;

namespace MessageSystem
{
class LoadResourceRequest;
}

struct LoadRequest
{
    size_t m_resourceType = 0;
    size_t m_gameObjectId = 0;
    void* m_loadData = nullptr;

#ifdef _DEBUG
    SourceInfo m_sourceInfo;
#endif
}; 

class ResourceLoader : public MessageSystem::IMessageDispatcher
{
public:
    ResourceLoader() {}
    ~ResourceLoader() {}

    void initialise(Resource* resource) { m_resource = resource; }
    void update();

    virtual void dispatchMessage(const MessageSystem::Message & msg) override;
    void ResourceLoader::AddLoadRequest(const LoadRequest& request);

    Resource* m_resource;
    std::vector<Job*> m_jobs;
    std::vector<Job*> m_newJobs;

    bool m_updating = false;
};