#include "Loader/ResourceLoadJobs.h"

#include "Core/MessageSystem/MessageQueue.h"
#include "Core/MessageSystem/RenderMessages.h"
#include "Core/Resource/RenderResource.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "Core/Types/TypeHelpers.h"
#include "Graphics/DeviceManager.h"
#include "Graphics/modelmanager.h"
#include "Graphics/texture.h"
#include "Graphics/texturemanager.h"

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void ResourceLoadJob::SendReturnMsg(size_t gameObjectId, size_t resourceHandle)
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
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void FaceJob::Execute(size_t threadIndex)
{
    UNUSEDPARAM(threadIndex);
    RenderResource& renderResource = RenderResourceHelper(m_resource).getWriteableResource();
    auto resourceHandle = renderResource.getModelManager().AddFace(m_loadData);//needs to be thread safe
    SendReturnMsg(m_gameObjectId, resourceHandle);
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void LoadTextureJob::Execute(size_t threadIndex)
{
    if (m_fileName.empty())
        return;

    RenderResource& renderResource = RenderResourceHelper(m_resource).getWriteableResource();

    //Extract filename if file name contains a path as well, this is not always true need to deal with relative paths here too
    std::string textureName = getTextureNameFromFileName(m_fileName);

    TextureManager& texManager = renderResource.getTextureManager();
    if (texManager.find(textureName))
    {
        return;
    }

    Texture texture;
    if (!texture.loadTextureFromFile(renderResource.getDeviceManager(), m_fileName))
    {
        MSG_TRACE_CHANNEL("ERROR", "Texture cannot be loaded: %s on thread: %d", m_fileName.c_str(), threadIndex);
        return;
    }

    texManager.addTexture(textureName, texture);
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void LoadModelJob::Execute(size_t threadIndex)
{
    UNUSEDPARAM(threadIndex);
    RenderResource& renderResource = RenderResourceHelper(m_resource).getWriteableResource();
    auto resourceHandle = renderResource.getModelManager().LoadModel(m_loadData);//needs to be thread safe, and should be a job
    SendReturnMsg(m_gameObjectId, resourceHandle);
}

