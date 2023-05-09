#include "Graphics/D3D12/MultithreadedCommandlistManager.h"

#include "Core/Resource/RenderResource.h"
#include "Graphics/DebugHelperFunctions.h"

#include <optick.h>

void MultithreadedCommandlistManager::Cleanup()
{
    auto& helper = RenderResourceHelper(m_resource).getWriteableResource();
    m_queueHandle = helper.getCommandQueueManager().CreateCommandQueue();
    CommandQueue& commandQueue = helper.getCommandQueueManager().GetCommandQueue(m_queueHandle);

    commandQueue.Cleanup();
}

void MultithreadedCommandlistManager::Initialise(Resource* resource, const std::string_view& name)
{
    m_resource = resource;

    //Should probably use a dubble buffered command list here
    auto& helper = RenderResourceHelper(m_resource).getWriteableResource();
    m_queueHandle = helper.getCommandQueueManager().CreateCommandQueue();
    CommandQueue& commandQueue = helper.getCommandQueueManager().GetCommandQueue(m_queueHandle);
    commandQueue.SetName(name);

    for (size_t index = 0; index < CommandQueue::MaxCommandLists; ++index)
    {
        commandQueue.CreateCommandList();
    }
}

bool MultithreadedCommandlistManager::GetCommandListHandleForThreadIndex(size_t threadIndex, CommandList& commandList)
{
    OPTICK_EVENT();

    ASSERT(threadIndex <= CommandQueue::MaxCommandLists, "Trying to grab a commandlist handle for a non buffered thread");

    //Deal with active set here and need to lock
    int expected = 1;
    if (m_commandListStatus[threadIndex].m_semaphore.compare_exchange_strong(expected, 0))
    {
        std::scoped_lock lock(m_mutex);

        auto& helper = RenderResourceHelper(m_resource).getWriteableResource();
        CommandQueue& commandQueue = helper.getCommandQueueManager().GetCommandQueue(m_queueHandle);
        commandList = commandQueue.GetCommandList(threadIndex);
        return true;
    }

    return false; //This means we cant service the job on this thread at this point so retun to queue
}

void MultithreadedCommandlistManager::ReturnCommandListForThreadIndex(size_t threadIndex)
{
    OPTICK_EVENT();

    ASSERT(threadIndex <= CommandQueue::MaxCommandLists / 2, "Trying to return a commandlist handle for a non buffered thread");
    std::scoped_lock lock(m_mutex);

    //Need to submit this commandlist
    auto& helper = RenderResourceHelper(m_resource).getWriteableResource();
    auto& resourceCommandQueue = helper.getCommandQueueManager().GetCommandQueue(m_queueHandle);

    HRESULT hr = E_FAIL;
    auto& resourceCommandList = resourceCommandQueue.GetCommandList(threadIndex);
    if (resourceCommandList.m_list != nullptr)
    {
        hr = resourceCommandList.m_list->Close();
        if (hr != S_OK)
        {
            MSG_TRACE_CHANNEL("ResourceLoader", "Failed to close the resource command list 0x%x %s", hr, getLastErrorMessage(hr));
        }

        ID3D12CommandList* const d3dCommandList = resourceCommandList.m_list;
        resourceCommandQueue.m_queue->ExecuteCommandLists(1u, &(d3dCommandList));
        UINT fence = resourceCommandQueue.m_fenceValue;
        hr = resourceCommandQueue.m_queue->Signal(resourceCommandQueue.m_fence, fence);
        if (hr != S_OK)
        {
            MSG_TRACE_CHANNEL("ERROR", "Fence Signal failed with error code: 0x%x %s", hr, getLastErrorMessage(hr));
        }
        resourceCommandQueue.m_fenceValue++;

        //See if our execution is done or not, this needs to be done in update too I think
        UINT64 fenceValue = resourceCommandQueue.m_fence->GetCompletedValue();
        if (fenceValue <= fence) //Gpu has not finished our work yet
        {
            m_commandListStatus[threadIndex].m_fenceValue = fenceValue; //When this value of the fence is passed we can decrement the semaphore.
            //Switch to secondary commandlist
            //m_commandListStatus[threadIndex].m_offset = CommandQueue::MaxCommandLists / 2;
            int expected = 0;
            m_commandListStatus[threadIndex].m_semaphore.compare_exchange_strong(expected, -1); //Tell the update, recording is done and as soon as the fence is bigger we can reset
        }
        else
        {
            //GPU is done, reset the command list
            hr = ResetCommandListAndAllocator(resourceCommandQueue, threadIndex);
            if (hr != S_OK)
            {
                MSG_TRACE_CHANNEL("ERROR", "Failed to reset commandlist with error code: 0x%x %s on thread: %d", hr, getLastErrorMessage(hr), threadIndex);
            }
            int expected = 0;
            m_commandListStatus[threadIndex].m_semaphore.compare_exchange_strong(expected, 1); //The list was reset, so make it available for other tasks on the thread.
            m_commandListStatus[threadIndex].m_fenceValue = fenceValue;
        }
    }
}

void MultithreadedCommandlistManager::Update()
{
    OPTICK_EVENT();

    std::scoped_lock lock(m_mutex);
    //Check the command queue complted state and reset the semaphores

    auto& helper = RenderResourceHelper(m_resource).getWriteableResource();
    auto& resourceCommandQueue = helper.getCommandQueueManager().GetCommandQueue(m_queueHandle);
    uint fenceValue = resourceCommandQueue.m_fence->GetCompletedValue();
    //any of the command list status that are below the fence value are done and need reseting on the semaphore
    size_t index = 0;
    for (auto& commandListStatus : m_commandListStatus)
    {
        if (fenceValue > commandListStatus.m_fenceValue)
        {
            //The problem here is that we want to reset a list if its not being recorded on anymore and the fence value is bigger than the commandlist value for the fence its waiting on.
            //The list sets the atomic to 0 when we are recording, we should probably set the list to -1 once we are done.
            int expected = -1;
            if (commandListStatus.m_semaphore.compare_exchange_strong(expected, 1))
            {

                if (ResetCommandListAndAllocator(resourceCommandQueue, index) != S_OK)
                {
                    MSG_TRACE_CHANNEL("ResourceLoader", "Failed to reset commandlist in disptach resource lists");
                }
                commandListStatus.m_fenceValue = fenceValue;
            }
        }
        ++index;
    }
}

HRESULT MultithreadedCommandlistManager::ResetCommandListAndAllocator(auto& resourceCommandQueue, size_t index)
{
    HRESULT hr = S_OK;
    auto& resourceCommandList = resourceCommandQueue.GetCommandList(index);
    if (resourceCommandList.m_list != nullptr)
    {
        hr = resourceCommandList.m_alloctor->Reset();
        if (hr != S_OK)
        {
            MSG_TRACE_CHANNEL("ResourceLoader", "Failed to reset the resource command list allocator 0x%x %s", hr, getLastErrorMessage(hr));
        }
        hr = resourceCommandList.m_list->Reset(resourceCommandList.m_alloctor, nullptr);
        if (hr != S_OK)
        {
            MSG_TRACE_CHANNEL("ResourceLoader", "Failed to reset the resource command list 0x%x %s", hr, getLastErrorMessage(hr));
        }

        //Callback for this index
        m_func(index);
    }

    return hr;
}
