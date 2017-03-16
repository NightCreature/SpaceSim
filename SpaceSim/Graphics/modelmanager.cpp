#include "Graphics/modelmanager.h"
#include "assert.h"
#include <iostream>
#include "Graphics/Model.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "Loader/ModelLoaders/AssimpModelLoader.h"
#include "Loader/ModelLoaders/XMLModelLoader.h"
#include "Loader/ModelLoaders/MmlLoader.h"
#include "Core/MessageSystem/MessageQueue.h"
#include "Core/MessageSystem/GameMessages.h"
#include "Core/MessageSystem/RenderMessages.h"
#include "Core/Resource/RenderResource.h"

#include "Gameplay/face.h"

ModelManager::~ModelManager()
{
    cleanup();
}

void ModelManager::cleanup()
{
    for (auto createdModel : m_models)
    {
        delete createdModel.m_model.model;
    }

    m_models.clear();
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
Model* ModelManager::LoadModel( Resource* resource, const ShaderInstance& shaderInstance, const std::string& fileName ) const
{
    if (fileName.empty())
    {
        return nullptr;
    }

    //unsigned int fileNameHash = hashString(fileName);
    //MSG_TRACE_CHANNEL("MODELMANAGER", "hash for %s is %d", fileName.c_str(), fileNameHash);
    //GeometryTreeIterator it = m_loadedModels.find(fileNameHash);
    //if (it != m_loadedModels.end())
    //{
    //    return it->second;
    //}

    std::string extension = extractExtensionFromFileName(fileName);

    Model* model = nullptr;
    if ( strICmp(extension, "dat") || strICmp(extension, "xml") )
    {
        XMLModelLoader loader;
        model = loader.LoadModel(resource, shaderInstance, fileName);
        //m_loadedModels.insert(GeometryTreePair(fileNameHash, model));
    }
    else if (strICmp(extension, "mml"))
    {
        //This format is a mixture between an xml and a attached model file
        MmlLoader loader;
        model = loader.LoadModel(resource, shaderInstance, fileName);
    }
    else
    {
        AssimpModelLoader loader;
        model = loader.LoadModel(resource, shaderInstance, fileName);
        //m_loadedModels.insert(GeometryTreePair(fileNameHash, model)); 
    }
    
    return model; //These should probably also register
}

void ModelManager::dispatchMessage(const MessageSystem::Message& msg)
{
    const MessageSystem::CreateRenderResourceMessage& crrmsg = static_cast<const MessageSystem::CreateRenderResourceMessage&>(msg);
    if (msg.getMessageId() == MESSAGE_ID(CreateFixedModelResource)) //Probably gonna need to something special about this message id since it is in heriting and we are subscribing to the base version
    {
        //we might want to queue this command
        if (crrmsg.GetResourceType() == hashString("Face::CreationParams"))
        {
            const MessageSystem::CreateFixedModelResource<Face::CreationParams>& faceMsg = static_cast<const MessageSystem::CreateFixedModelResource<Face::CreationParams>&>(msg);
            auto creationParams = faceMsg.GetData();
            size_t renderResourceId = HASH_BINARY(creationParams);
            if (!HasRenderResource(renderResourceId))
            {
                //register face with model manager
                RegisterCreatedModel(Face::CreateFace(creationParams->m_fixedData, m_resource), renderResourceId);
            }
            else
            {
                //const CreatedModel* model = GetRenderResource(renderResourceId);                
            }
            //Create Retrun message giving GO link to this render resource, Need to delay add this to the queue again or message is lost
            MessageSystem::CreatedRenderResourceMessage returnMessage;
            MessageSystem::CreatedRenderResourceMessage::CreatedRenderResource data;
            data.m_gameObjectId = faceMsg.GetGameObjectId();
            data.m_renderResourceHandle = renderResourceId;
            returnMessage.SetData(data);
            //Send this message
            RenderResourceHelper(m_resource).getWriteableResource().m_messageQueues->getRenderMessageQueue()->addMessage(returnMessage); //This will be deleted sadly
        }
    }
}

//-----------------------------------------------------------------------------
//! @brief   Initialise the application
//! @remark
//-----------------------------------------------------------------------------
bool ModelManager::HasRenderResource(size_t resource_id) const
{
    return (std::find_if(cbegin(m_models), cend(m_models), [resource_id](const auto& entry) { return entry.m_resourceId == resource_id; })) != cend(m_models);

}

//-----------------------------------------------------------------------------
//! @brief   Initialise the application
//! @remark
//-----------------------------------------------------------------------------
void ModelManager::RegisterCreatedModel(CreatedModel model, size_t renderResourceId)
{
    if (!HasRenderResource(renderResourceId))
    {
        ModelResourceHandle handle;
        handle.m_model = model;
        handle.m_resourceId = renderResourceId;

        m_models.push_back(handle);
    }
}

//-----------------------------------------------------------------------------
//! @brief   Initialise the application
//! @remark
//-----------------------------------------------------------------------------
const CreatedModel* ModelManager::GetRenderResource(size_t renderResourceId) const
{
    ModelsArray::const_iterator it = std::find_if(cbegin(m_models), cend(m_models), [renderResourceId](const auto& entry) { return entry.m_resourceId == renderResourceId; });
    if (it != cend(m_models))
    {
        return &(it->m_model);
    }

    return nullptr;
}
