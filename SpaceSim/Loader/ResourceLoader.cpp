#include "Loader/ResourceLoader.h"

#include "Core/MessageSystem/GameMessages.h"
#include "Core/MessageSystem/MessageQueue.h"
#include "Core/MessageSystem/RenderMessages.h"
#include "Core/Resource/RenderResource.h"

#include "Graphics/modelmanager.h"

//-----------------------------------------------------------------------------
//! @brief   Initialise the application
//! @remark
//-----------------------------------------------------------------------------
void ResourceLoader::update()
{
    BROFILER_CATEGORY("ResourceLoader::update", Profiler::Color::Crimson);

    RenderResource& renderResource = RenderResourceHelper(m_resource).getWriteableResource();

    for (auto request : m_requests)
    {
        if (request.m_resourceType == hashString("Face::CreationParams"))
        {
            MessageSystem::CreatedRenderResourceMessage returnMessage;
            MessageSystem::CreatedRenderResourceMessage::CreatedRenderResource data;
            data.m_renderResourceHandle = renderResource.getModelManager().AddFace(request.m_loadData);
            data.m_gameObjectId = request.m_gameObjectId;
            returnMessage.SetData(data);
            
            renderResource.m_messageQueues->getRenderMessageQueue()->addMessage(returnMessage);
        }
    }
}

//-----------------------------------------------------------------------------
//! @brief   Initialise the application
//! @remark
//-----------------------------------------------------------------------------
void ResourceLoader::dispatchMessage(const MessageSystem::Message & msg)
{
    const MessageSystem::LoadResourceRequest& crrmsg = static_cast<const MessageSystem::LoadResourceRequest&>(msg);
    //Figure out what kind of load request and schedule it to be loaded
    LoadRequest request;
    request.m_resourceType = crrmsg.GetResourceType(); //should probably store load requests by type
    request.m_gameObjectId = crrmsg.GetGameObjectId();
    request.m_loadData = static_cast<void*>(new char[crrmsg.GetImplementationDataSize()]);
    memcpy(request.m_loadData, crrmsg.GetImplementationData(), crrmsg.GetImplementationDataSize());

    m_requests.push_back(request);
}
