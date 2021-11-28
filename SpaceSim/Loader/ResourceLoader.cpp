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


///-----------------------------------------------------------------------------
///! @brief   
///! @remark This needs to run after we have a device now
///-----------------------------------------------------------------------------
void ResourceLoader::initialise(Resource* resource)
{
    m_resource = resource;

    //Should probably use a dubble buffered command list here
    auto& helper = RenderResourceHelper(m_resource).getWriteableResource();
    m_uploadQueueHandle = helper.getDeviceManager().CreateCommandQueue();
    CommandQueue& commandQueue = helper.getDeviceManager().GetCommandQueue(m_uploadQueueHandle);
    m_currentUploadCommandListHandle = commandQueue.CreateCommandList();
    m_previousCommandListHandle = commandQueue.CreateCommandList();
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
///! @remark
///-----------------------------------------------------------------------------
void ResourceLoader::DispatchResourceCommandQueue()
{
    auto& helper = RenderResourceHelper(m_resource).getWriteableResource();

    auto& resourceCommandQueue = helper.getDeviceManager().GetCommandQueue(m_uploadQueueHandle);
    auto& resourceCommandList = resourceCommandQueue.GetCommandList(m_currentUploadCommandListHandle);

    HRESULT hr = resourceCommandList.m_list->Close();
    if (hr != S_OK)
    {
        MSG_TRACE_CHANNEL("ResourceLoader", "Failed to close the resource command list 0x%x %s", hr, getLastErrorMessage(hr));
    }

    std::vector<ID3D12CommandList*> resourceCommandListsVector;
    resourceCommandListsVector.push_back(resourceCommandList.m_list); //Push back the current command list

    resourceCommandQueue.m_queue->ExecuteCommandLists(static_cast<UINT>(resourceCommandListsVector.size()), &resourceCommandListsVector[0]);

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

    auto& commandList = helper.getDeviceManager().GetCommandQueue(m_uploadQueueHandle).GetCommandList(m_currentUploadCommandListHandle);
    commandList.m_alloctor->Reset();
    commandList.m_list->Reset(commandList.m_alloctor, nullptr);

    size_t tempCommandListIndex = m_currentUploadCommandListHandle;
    m_currentUploadCommandListHandle = m_previousCommandListHandle;
    m_previousCommandListHandle = tempCommandListIndex;

    //Before we do this we need to know that the previous commandlist has been finished, so we can reset the list.

}

///-----------------------------------------------------------------------------
///! @brief   Create different load jobs per type
///! @remark
///-----------------------------------------------------------------------------
void ResourceLoader::AddLoadRequest(const LoadRequest& request)
{
    // These jobs will have to wait untill the commandlist they have put there commands in is done on the GPU before sending messages back to the update side
    Job* job = nullptr;
    switch (request.m_resourceType)
    {
    case "Face::CreationParams"_hash:
        job = new FaceJob(m_resource, m_uploadQueueHandle, m_currentUploadCommandListHandle, request.m_gameObjectId, request.m_loadData);
    break;
    case "LOAD_TEXTURE"_hash:
    {
        job = new LoadTextureJob(m_resource, m_uploadQueueHandle, m_currentUploadCommandListHandle, (char*)(request.m_loadData));
        delete request.m_loadData;
    }
    break;
    case "LoadModelResource"_hash:
        job = new LoadModelJob(m_resource, m_uploadQueueHandle, m_currentUploadCommandListHandle, request.m_gameObjectId, request.m_loadData);
    break;
    default:
        break;
    }

    if (job != nullptr)
    {
        //RenderResource& renderResource = RenderResourceHelper(m_resource).getWriteableResource();
        //renderResource.getJobQueue().AddJob(job);
        if (m_updating)
        { 
            m_newJobs.push_back(job);
        }
        else
        {
            m_jobs.push_back(job);
        }

    }
}