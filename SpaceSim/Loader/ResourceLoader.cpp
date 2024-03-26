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
#include <chrono>


///-----------------------------------------------------------------------------
///! @brief   
///! @remark This needs to run after we have a device now
///-----------------------------------------------------------------------------
void ResourceLoader::initialise(Resource* resource)
{
    m_resource = resource;
    m_commandListManager.Initialise(resource, "ResourceCommandLists");
    m_commandListManager.SetCallbackOnCommandlistFinished([this](size_t index) 
        {    
            std::scoped_lock lock(m_mutex);
            for (auto pair : m_returnMessageDataPerThread[index])
            {
                SendReturnMsg(pair.first, pair.second);
            }

            m_returnMessageDataPerThread[index].clear();
        });
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void ResourceLoader::update()
{
    m_updating = true;

    //for (auto* job : m_newJobs)
    //{
    //    m_jobs.push_back(job);
    //}
    //m_newJobs.clear();

    //for (auto* job : m_jobs)
    //{
    //    job->Execute(0);
    //    delete job;
    //}

    //m_jobs.clear();


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
    LoadRequest request(reinterpret_cast<byte*>(crrmsg.GetImplementationData()), crrmsg.GetImplementationDataSize());
    request.m_resourceType = crrmsg.GetResourceType(); //should probably store load requests by type
    request.m_gameObjectId = crrmsg.GetGameObjectId();

#ifdef _DEBUG
    //request.m_sourceInfo = SourceInfo(msg.m_sourceInfo.getSourceFileName().c_str(), msg.m_sourceInfo.getSourceFileLineNumber());
#endif

    AddLoadRequest(std::move(request));
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark This needs to know when the jobs it pushed in to the system are done so we can dispatch the command queue. Maybe we create new commandqueues for each job that submit them when they are done with their job
///-----------------------------------------------------------------------------
void ResourceLoader::DispatchResourceCommandQueue()
{
    m_commandListManager.Update();
}

bool ResourceLoader::GetCommandListHandleForThreadIndex(size_t threadIndex, CommandList& commandList)
{
    return m_commandListManager.GetCommandListHandleForThreadIndex(threadIndex, commandList);
}

void ResourceLoader::ReturnCommandListForThreadIndex(size_t threadIndex)
{
    m_commandListManager.ReturnCommandListForThreadIndex(threadIndex);
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void ResourceLoader::AddedReturnMessageDataForThreadIndex(size_t threadIndex, size_t gameObjectId, size_t resourceHandle)
{
    std::scoped_lock lock(m_mutex);
    m_returnMessageDataPerThread[threadIndex].push_back(std::make_pair(gameObjectId, resourceHandle));
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark This might need to change to a slot of bools and then update sends the return messages
///-----------------------------------------------------------------------------
void ResourceLoader::CallbackCommandListFinished(size_t threadIndex)
{
    std::scoped_lock lock(m_mutex);
    for (auto pair : m_returnMessageDataPerThread[threadIndex])
    {
        SendReturnMsg(pair.first, pair.second);
    }

    m_returnMessageDataPerThread[threadIndex].clear();
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void ResourceLoader::SendReturnMsg(size_t gameObjectId, size_t resourceHandle)

{
    MessageSystem::CreatedRenderResourceMessage returnMessage;
    MessageSystem::CreatedRenderResourceMessage::CreatedRenderResource data;
    data.m_renderResourceHandle = resourceHandle;
    data.m_gameObjectId = gameObjectId;
    returnMessage.SetData(data);

    RenderResource& renderResource = RenderResourceHelper(m_resource).getWriteableResource();
    renderResource.m_messageQueues->getRenderMessageQueue()->addMessage(returnMessage);
}

///-----------------------------------------------------------------------------
///! @brief   Create different load jobs per type
///! @remark
///-----------------------------------------------------------------------------
void ResourceLoader::AddLoadRequest(LoadRequest&& request)
{
    // These jobs will have to wait until the commandlist they have put there commands in is done on the GPU before sending messages back to the update side
    ResourceLoadJob* job = nullptr;
    switch (request.m_resourceType)
    {
    case "Face::CreationParams"_hash:
        job = new FaceJob(std::move(request), this);
        break;
    case "LOAD_TEXTURE"_hash:
    {
        job = new LoadTextureJob(std::move(request), this);
    }
    break;
    case "LoadModelResource"_hash:
        job = new LoadModelJob(std::move(request), this);
        break;
    case "LoadTextureListRequest"_hash:
    {
        job = new LoadTextureListJob(std::move(request), this);
    }
        break;
    default:
        break;
    }

    RenderResource& renderResource = RenderResourceHelper(m_resource).getWriteableResource();
    renderResource.getJobQueue().AddJob(job);
}