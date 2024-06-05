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
#include "Core/Profiler/ProfilerMacros.h"
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
    PROFILE_FUNCTION();

    RenderResource& renderResource = RenderResourceHelper(context->m_renderResource).getWriteableResource();
    CommandList commandList;
    CommandQueue* commandQueue = nullptr;
    bool retVal = m_loader->GetCommandListHandleForThreadIndex(context->m_threadIndex, commandList, commandQueue);
    if (retVal)
    {
        m_resourceHandle = renderResource.getModelManager().AddFace(m_request.m_loadData.GetData<void>(), commandList, this);
        m_loader->AddedReturnMessageDataForThreadIndex(context->m_threadIndex, m_request.m_gameObjectId, m_resourceHandle);
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
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
bool LoadTextureJob::Execute(ThreadContext* context)
{
    PROFILE_FUNCTION();

    CommandList commandList;
    CommandQueue* commandQueue = nullptr;
    bool retVal = m_loader->GetCommandListHandleForThreadIndex(context->m_threadIndex, commandList, commandQueue);
    if (retVal)
    {
        RenderResource& renderResource = RenderResourceHelper(context->m_renderResource).getWriteableResource();

        //Extract filename if file name contains a path as well, this is not always true need to deal with relative paths here too
        const auto* fileName = m_request.m_loadData.GetData<std::string>();
        std::string textureName = getTextureNameFromFileName(*fileName);

        TextureManager& texManager = renderResource.getTextureManager();
        auto textureInformation = texManager.AddOrCreateTexture(textureName);
        if (textureInformation.m_heapIndex == DescriptorHeap::invalidDescriptorIndex && textureInformation.m_loadRequested == false)
        {
            Texture12 texture;
            size_t descriptorIndex = DescriptorHeap::invalidDescriptorIndex;
            if (!texture.loadTextureFromFile(renderResource.getDeviceManager(), commandList, *commandQueue, *fileName, renderResource.getDescriptorHeapManager().GetSRVCBVUAVHeap().GetCPUDescriptorHandle(descriptorIndex)))
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
    PROFILE_FUNCTION();

    CommandList commandList;
    CommandQueue* commandQueue = nullptr;
    bool retVal = m_loader->GetCommandListHandleForThreadIndex(context->m_threadIndex, commandList, commandQueue);
    if (retVal)
    {
        RenderResource& renderResource = RenderResourceHelper(context->m_renderResource).getWriteableResource();

        auto resourceHandle = renderResource.getModelManager().LoadModel(m_request.m_loadData.GetData<void>(), commandList);//needs to be thread safe, and should be a job
        m_loader->AddedReturnMessageDataForThreadIndex(context->m_threadIndex, m_request.m_gameObjectId, resourceHandle);
    }

    return retVal;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
bool LoadTextureListJob::Execute(ThreadContext* context)
{
    PROFILE_FUNCTION();

    const auto* textureList = m_request.m_loadData.GetData<std::vector<std::filesystem::path>>();
    if (textureList != nullptr && textureList->empty())
    {
        return true; //do nothing
    }

	CommandList commandList;
    CommandQueue* commandQueue = nullptr;
    bool retVal = m_loader->GetCommandListHandleForThreadIndex(context->m_threadIndex, commandList, commandQueue);
	if (retVal)
	{
        for (const auto& filePath : *textureList)
        {
            RenderResource& renderResource = RenderResourceHelper(context->m_renderResource).getWriteableResource();

            //Extract filename if file name contains a path as well, this is not always true need to deal with relative paths here too
            std::string textureName = getTextureNameFromFileName(filePath.string());

            TextureManager& texManager = renderResource.getTextureManager();
            auto textureInformation = texManager.AddOrCreateTexture(textureName);
            if (textureInformation.m_heapIndex == DescriptorHeap::invalidDescriptorIndex && textureInformation.m_loadRequested == false)
            {
                Texture12 texture;
                size_t descriptorIndex = DescriptorHeap::invalidDescriptorIndex;
                if (!texture.loadTextureFromFile(renderResource.getDeviceManager(), commandList, *commandQueue, filePath.string(), renderResource.getDescriptorHeapManager().GetSRVCBVUAVHeap().GetCPUDescriptorHandle(descriptorIndex)))
                {
                    MSG_TRACE_CHANNEL("ERROR", "Texture cannot be loaded: %s on thread: %d", textureName.c_str(), context->m_threadIndex);
                    return true;
                }

                texManager.addTexture(textureName, texture, descriptorIndex);
            }
        }
	}

	return retVal;
}