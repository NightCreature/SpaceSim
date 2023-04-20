#include "Loader/ResourceLoadJobs.h"

#include "Core/MessageSystem/MessageQueue.h"
#include "Core/MessageSystem/RenderMessages.h"
#include "Core/Resource/RenderResource.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "Core/Types/TypeHelpers.h"
#include "Graphics/DeviceManager.h"
#include "Graphics/modelmanager.h"
#include "Graphics/texture12.h"
//#include "Graphics/texture.h"
#include "Graphics/texturemanager.h"
#include "Core/MessageSystem/GameMessages.h"

#include "Loader/ResourceLoader.h"
#include "Core/Thread/JobSystem.h"
//None of this can send a return msg until the command list has been executed on the GPU


///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void ResourceLoadJob::Finish(ThreadContext* context)
{
    //m_loader->CompleteCommandList(m_commandListHandle);
    m_loader->ReturnCommandListForThreadIndex(context->m_threadIndex);
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void ResourceLoadJob::SendReturnMsg(Resource* resource, size_t gameObjectId, size_t resourceHandle)
{
    MessageSystem::CreatedRenderResourceMessage returnMessage;
    MessageSystem::CreatedRenderResourceMessage::CreatedRenderResource data;
    data.m_renderResourceHandle = resourceHandle;
    data.m_gameObjectId = gameObjectId;
    returnMessage.SetData(data);

    RenderResource& renderResource = RenderResourceHelper(resource).getWriteableResource();
    renderResource.m_messageQueues->getRenderMessageQueue()->addMessage(returnMessage);
}

CommandList* ResourceLoadJob::GetCommandList()
{
    //If we cant get the commandlist we cant handle the job yet
    //m_loader->GetCommandListHandleForThreadIndex();
    
    return nullptr;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
bool FaceJob::Execute(ThreadContext* context)
{
    OPTICK_EVENT();

    RenderResource& renderResource = RenderResourceHelper(context->m_renderResource).getWriteableResource();
    CommandList commandList;
    bool retVal = m_loader->GetCommandListHandleForThreadIndex(context->m_threadIndex, commandList);
    if (retVal)
    {
        m_resourceHandle = renderResource.getModelManager().AddFace(m_loadData, commandList, this);
        SendReturnMsg(context->m_renderResource, m_gameObjectId, m_resourceHandle);
    }

    return retVal;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void FaceJob::Finish(ThreadContext* context)
{
    ResourceLoadJob::Finish(context);
    //Since we handled the loading of our data get rid of the jobs data
    delete m_loadData;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
bool LoadTextureJob::Execute(ThreadContext* context)
{
    OPTICK_EVENT();

    if (m_fileName.empty())
        return true; //do nothing

    CommandList commandList;
    bool retVal = m_loader->GetCommandListHandleForThreadIndex(context->m_threadIndex, commandList);
    if (retVal)
    {
        RenderResource& renderResource = RenderResourceHelper(context->m_renderResource).getWriteableResource();

        //Extract filename if file name contains a path as well, this is not always true need to deal with relative paths here too
        std::string textureName = getTextureNameFromFileName(m_fileName);

        TextureManager& texManager = renderResource.getTextureManager();
        auto& textureInformation = texManager.AddOrCreateTexture(textureName);
        if (textureInformation.m_heapIndex == DescriptorHeap::invalidDescriptorIndex && textureInformation.m_loadRequested == false)
        {
            Texture12 texture;
            size_t descriptorIndex = DescriptorHeap::invalidDescriptorIndex;
            if (!texture.loadTextureFromFile(renderResource.getDeviceManager(), commandList, m_fileName, renderResource.getDescriptorHeapManager().GetSRVCBVUAVHeap().GetCPUDescriptorHandle(descriptorIndex)))
            {
                MSG_TRACE_CHANNEL("ERROR", "Texture cannot be loaded: %s on thread: %d", textureName.c_str(), context->m_threadIndex);
                return true;
            }

            texManager.addTexture(textureName, texture, descriptorIndex);
        }
    }

    return retVal;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
bool LoadModelJob::Execute(ThreadContext* context)
{
    OPTICK_EVENT();

    CommandList commandList;
    bool retVal = m_loader->GetCommandListHandleForThreadIndex(context->m_threadIndex, commandList);
    if (retVal)
    {
        RenderResource& renderResource = RenderResourceHelper(context->m_renderResource).getWriteableResource();

        auto resourceHandle = renderResource.getModelManager().LoadModel(m_loadData, commandList);//needs to be thread safe, and should be a job
        SendReturnMsg(context->m_renderResource, m_gameObjectId, resourceHandle);

        delete m_loadData;
    }

    return retVal;
}