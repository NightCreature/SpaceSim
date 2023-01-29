#pragma once

#include "Core/MessageSystem/Messages.h"
#include "Core/Thread/Job.h"

#include <vector>
#include "Graphics/D3D12/CommandQueue.h"
#include <list>
#include <mutex>
#include <deque>

class Resource;
class ResourceLoadJob;

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

    void initialise(Resource* resource);
    void update();

    virtual void dispatchMessage(const MessageSystem::Message & msg) override;
    void AddLoadRequest(const LoadRequest& request);

    void DispatchResourceCommandQueue();

    void CompleteCommandList(size_t commandListHandle);
    size_t GetCommandQueueHandle() const { return m_uploadQueueHandle; }
    size_t GetCommandListHandle();

private:

    Resource* m_resource;
    std::vector<ResourceLoadJob*> m_jobs;
    std::deque<ResourceLoadJob*> m_newJobs;

    std::list<size_t> m_freeCommandLists;
    std::list<size_t> m_commandListsToProcess;

    std::mutex m_mutex;

    size_t m_uploadQueueHandle = ~0ull;
    size_t m_currentUploadCommandListHandle = ~0ull;
    size_t m_previousCommandListHandle = ~0ull;

    bool m_updating = false;
};