#include "Loader/ResourceLoader.h"

#include "Core/MessageSystem/GameMessages.h"
#include "Core/MessageSystem/MessageQueue.h"
#include "Core/MessageSystem/RenderMessages.h"
#include "Core/Resource/RenderResource.h"

#include "Graphics/DeviceManager.h"
#include "Graphics/modelmanager.h"
#include "Graphics/texturemanager.h"

#include "Loader/ResourceLoadJobs.h"
#include "Loader/ResourceLoadRequests.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include <mutex>


///-----------------------------------------------------------------------------
///! @brief   
///! @remark This needs to run after we have a device now
///-----------------------------------------------------------------------------
void ResourceLoader::initialise(Resource* resource)
{
    m_resource = resource;

    //Should probably use a dubble buffered command list here
    auto& helper = RenderResourceHelper(m_resource).getWriteableResource();
    m_uploadQueueHandle = helper.getCommandQueueManager().CreateCommandQueue();
    CommandQueue& commandQueue = helper.getCommandQueueManager().GetCommandQueue(m_uploadQueueHandle);
    commandQueue.SetName("ResourceLoaderQueue");

    for (size_t freeCommandListIndex = 0; freeCommandListIndex < commandQueue.m_commandLists.size(); ++freeCommandListIndex)
    {
        m_freeCommandLists.push_back(commandQueue.CreateCommandList());
    }
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void ResourceLoader::update()
{
    m_updating = true;

    for (auto* job : m_newJobs)
    {
        m_jobs.push_back(job);
    }
    m_newJobs.clear();

    for (auto* job : m_jobs)
    {
        job->Execute(0);
        delete job;
    }

    m_jobs.clear();


    m_updating = false;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void ResourceLoader::dispatchMessage(const MessageSystem::Message & msg)
{
    const MessageSystem::LoadResourceRequest& crrmsg = static_cast<const MessageSystem::LoadResourceRequest&>(msg);

    //Figure out what kind of load request and schedule it to be loader
    LoadRequest request;
    request.m_resourceType = crrmsg.GetResourceType(); //should probably store load requests by type
    request.m_gameObjectId = crrmsg.GetGameObjectId();
    request.m_loadData = static_cast<void*>(new char[crrmsg.GetImplementationDataSize()]);
    memcpy(request.m_loadData, crrmsg.GetImplementationData(), crrmsg.GetImplementationDataSize());

#ifdef _DEBUG
    request.m_sourceInfo = SourceInfo(msg.m_sourceInfo.getSourceFileName().c_str(), msg.m_sourceInfo.getSourceFileLineNumber());
#endif

    AddLoadRequest(request);
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark This needs to know when the jobs it pushed in to the system are done so we can dispatch the command queue. Maybe we create new commandqueues for each job that submit them when they are done with their job
///-----------------------------------------------------------------------------
void ResourceLoader::DispatchResourceCommandQueue()
{
    std::scoped_lock lock(m_mutex);
    if (!m_commandListsToProcess.empty())
    {
        auto& helper = RenderResourceHelper(m_resource).getWriteableResource();
        auto& resourceCommandQueue = helper.getCommandQueueManager().GetCommandQueue(m_uploadQueueHandle);

        std::vector<ID3D12CommandList*> commandLists;
        HRESULT hr = E_FAIL;
        for (size_t commandListHandle : m_commandListsToProcess)
        {
            auto& resourceCommandList = resourceCommandQueue.GetCommandList(commandListHandle);
            commandLists.push_back(resourceCommandList.m_list);

            hr = resourceCommandList.m_list->Close();
            if (hr != S_OK)
            {
                MSG_TRACE_CHANNEL("ResourceLoader", "Failed to close the resource command list 0x%x %s", hr, getLastErrorMessage(hr));
            }
        }

        resourceCommandQueue.m_queue->ExecuteCommandLists(static_cast<UINT>(commandLists.size()), &commandLists[0]);

        //have to wait for the GPU to be done this is not the best but fuck it for now
        const UINT64 fence = resourceCommandQueue.m_fenceValue;
        hr = resourceCommandQueue.m_queue->Signal(resourceCommandQueue.m_fence, fence);
        if (hr != S_OK)
        {
            MSG_TRACE_CHANNEL("ERROR", "Fence Signal failed with error code: 0x%x %s", hr, getLastErrorMessage(hr));
        }
        resourceCommandQueue.m_fenceValue++;

        // Wait until the previous frame is finished.
        UINT64 fenceValue = resourceCommandQueue.m_fence->GetCompletedValue();
        if (fenceValue < fence)
        {
            resourceCommandQueue.m_fence->SetEventOnCompletion(fence, resourceCommandQueue.m_fenceEvent);
            WaitForSingleObject(resourceCommandQueue.m_fenceEvent, INFINITE); //This could stall
        }

        for (size_t commandListHandle : m_commandListsToProcess)
        {
            auto& commandList = helper.getCommandQueueManager().GetCommandQueue(m_uploadQueueHandle).GetCommandList(commandListHandle);
            commandList.m_alloctor->Reset();
            commandList.m_list->Reset(commandList.m_alloctor, nullptr);
            //If we have no outstanding jobs free up this index for commands otherwise setup the new job
            if (!m_newJobs.empty())
            {
                ResourceLoadJob* job = m_newJobs.front();
                m_newJobs.pop_front();
                job->SetCommandListHandle(commandListHandle);
            }
            else
            {
                m_freeCommandLists.push_back(commandListHandle);
            }
        }
        m_commandListsToProcess.clear();

    }

}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void ResourceLoader::CompleteCommandList(size_t commandListHandle)
{
    std::scoped_lock lock(m_mutex);
    m_commandListsToProcess.push_back(commandListHandle);
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
size_t ResourceLoader::GetCommandListHandle()
{
    std::scoped_lock lock(m_mutex);
    if (!m_freeCommandLists.empty())
    {
        size_t commandHandle = m_freeCommandLists.front();
        m_freeCommandLists.pop_front();
        return commandHandle;
    }

    return CommandQueue::InvalidCommandListHandle;
}

///-----------------------------------------------------------------------------
///! @brief   Create different load jobs per type
///! @remark
///-----------------------------------------------------------------------------
void ResourceLoader::AddLoadRequest(const LoadRequest& request)
{
    // These jobs will have to wait until the commandlist they have put there commands in is done on the GPU before sending messages back to the update side
    size_t commandListHandle = GetCommandListHandle();
    ResourceLoadJob* job = nullptr;
    switch (request.m_resourceType)
    {
    case "Face::CreationParams"_hash:
        job = new FaceJob(m_resource, m_uploadQueueHandle, commandListHandle, request.m_gameObjectId, request.m_loadData, this);
        break;
    case "LOAD_TEXTURE"_hash:
    {
        job = new LoadTextureJob(m_resource, m_uploadQueueHandle, commandListHandle, (char*)(request.m_loadData), this);
        delete request.m_loadData;
    }
    break;
    case "LoadModelResource"_hash:
        job = new LoadModelJob(m_resource, m_uploadQueueHandle, commandListHandle, request.m_gameObjectId, request.m_loadData, this);
        break;
    default:
        break;
    }
    if (commandListHandle != CommandQueue::InvalidCommandListHandle)
    {
        if (job != nullptr)
        {
            RenderResource& renderResource = RenderResourceHelper(m_resource).getWriteableResource();
            renderResource.getJobQueue().AddJob(job);

        }
    }
    else
    {
        //These jobs have been created with an invalid handle and as such we can dispatch them until a queue is free, this probably will have to change
        m_newJobs.push_back(job);
    }
}