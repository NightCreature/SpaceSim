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

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void ResourceLoader::update()
{

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
///! @brief   Create different load jobs per type
///! @remark
///-----------------------------------------------------------------------------
void ResourceLoader::AddLoadRequest(const LoadRequest& request)
{
    Job* job = nullptr;
    switch (request.m_resourceType)
    {
    case "Face::CreationParams"_hash:
        job = new FaceJob(m_resource, request.m_gameObjectId, request.m_loadData);
    break;
    case "LOAD_TEXTURE"_hash:
        job = new LoadTextureJob(m_resource, (char*)(request.m_loadData));
    break;
    case "LoadModelResource"_hash:
        job = new LoadModelJob(m_resource, request.m_gameObjectId, request.m_loadData);
    break;
    default:
        break;
    }

    if (job != nullptr)
    {
        RenderResource& renderResource = RenderResourceHelper(m_resource).getWriteableResource();
        renderResource.getJobQueue().AddJob(job);
    }
}
