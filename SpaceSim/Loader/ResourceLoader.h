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
    void AddLoadRequest(LoadRequest&& request);

    void DispatchResourceCommandQueue();

    bool GetCommandListHandleForThreadIndex(size_t threadIndex, CommandList& commandList);
    void ReturnCommandListForThreadIndex(size_t threadIndex);

    void AddedReturnMessageDataForThreadIndex(size_t m_threadIndex, size_t m_gameObjectId, size_t resourceHandle);

    void CallbackCommandListFinished(size_t threadIndex);
private:

    void SendReturnMsg(size_t gameObjectId, size_t resourceHandle);

    Resource* m_resource;
    std::vector<ResourceLoadJob*> m_jobs;
    std::deque<ResourceLoadJob*> m_newJobs;

    std::mutex m_mutex;

    MultithreadedCommandlistManager m_commandListManager;
    //List of Gameobject Id to Render Handle
    using ListOfReturnData = std::vector<std::pair<size_t, size_t>>;
    std::array<ListOfReturnData, CommandQueue::MaxCommandLists> m_returnMessageDataPerThread;

    bool m_updating = false;
};