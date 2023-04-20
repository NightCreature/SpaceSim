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
///! @brief   Create different load jobs per type
///! @remark
///-----------------------------------------------------------------------------
void ResourceLoader::AddLoadRequest(const LoadRequest& request)
{
    // These jobs will have to wait until the commandlist they have put there commands in is done on the GPU before sending messages back to the update side
    ResourceLoadJob* job = nullptr;
    switch (request.m_resourceType)
    {
    case "Face::CreationParams"_hash:
        job = new FaceJob(request.m_gameObjectId, request.m_loadData, this);
        break;
    case "LOAD_TEXTURE"_hash:
    {
        job = new LoadTextureJob((char*)(request.m_loadData), this);
        delete request.m_loadData;
    }
    break;
    case "LoadModelResource"_hash:
        job = new LoadModelJob(request.m_gameObjectId, request.m_loadData, this);
        break;
    default:
        break;
    }

    RenderResource& renderResource = RenderResourceHelper(m_resource).getWriteableResource();
    renderResource.getJobQueue().AddJob(job);
}