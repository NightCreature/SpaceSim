#pragma once

#include "Core/MessageSystem/Messages.h"
#include "Core/Thread/Job.h"

#include <vector>
#include "Graphics/D3D12/CommandQueue.h"
#include <list>
#include <mutex>
#include <deque>
#include "Graphics/DebugHelperFunctions.h"
#include <semaphore>
#include "Graphics/D3D12/MultithreadedCommandlistManager.h"

class Resource;
class ResourceLoadJob;
struct LoadRequest;

namespace MessageSystem
{
class LoadResourceRequest;
}

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

    bool GetCommandListHandleForThreadIndex(size_t threadIndex, CommandList& commandList);
    void ReturnCommandListForThreadIndex(size_t threadIndex);

private:

    Resource* m_resource;
    std::vector<ResourceLoadJob*> m_jobs;
    std::deque<ResourceLoadJob*> m_newJobs;

    std::mutex m_mutex;

    MultithreadedCommandlistManager m_commandListManager;

    bool m_updating = false;
};