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

#include "Loader/ModelLoaders/ModelLoader.h"
#include "Core/MessageSystem/GameMessages.h"

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

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
size_t ModelManager::LoadModel( void* data, size_t commandQueueHandle, size_t commandLisHandle)
{
    auto modelData = static_cast<MessageSystem::CreateRenderResource<LoadModelResource>::ResourceData<LoadModelResource>*>(data);

    ModelLoader::LoadData loadData;
    loadData.m_fileName = modelData->m_fixedData.m_fileName;
    size_t renderResourceId = hashString(loadData.m_fileName);
    if (!HasRenderResource(renderResourceId) && renderResourceId)
    {
        if (loadData.m_fileName.empty())
        {
            return renderResourceId;
        }

        std::string extension = extractExtensionFromFileName(loadData.m_fileName);

        auto& resource = RenderResourceHelper(m_resource).getWriteableResource();
        auto& commandQueue = resource.getCommandQueueManager().GetCommandQueue(commandQueueHandle);
        auto& commandList = commandQueue.GetCommandList(commandLisHandle);

        CreatedModel createdModel;
        if (strICmp(extension, "dat") || strICmp(extension, "xml"))
        {
            XMLModelLoader loader;
            createdModel = loader.LoadModel(m_resource, loadData, commandList);
            //m_loadedModels.insert(GeometryTreePair(fileNameHash, model));
        }
        else if (strICmp(extension, "mml"))
        {
            //This format is a mixture between an xml and a attached model file
            MmlLoader loader;
            createdModel = loader.LoadModel(m_resource, loadData, commandList);
        }
        else
        {
            MSG_TRACE_CHANNEL("ModelManager", "unsupported file format encountered for file name: %s", loadData.m_fileName.c_str());
        }

        if (createdModel.model != nullptr)
        {
             //Need to deal with the bounding box here too really
            RegisterCreatedModel(createdModel, renderResourceId);
        }
    }

    return renderResourceId;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
size_t ModelManager::AddFace(void* data, size_t commandQueueHandle, size_t commandLisHandle)
{
    auto* creationParams = static_cast<MessageSystem::CreateRenderResource<Face::CreationParams>::ResourceData<Face::CreationParams>*>(data);
    size_t renderResourceId = HASH_BINARY(creationParams);
    if (!HasRenderResource(renderResourceId))
    {
        auto& resource = RenderResourceHelper(m_resource).getWriteableResource();
        auto& commandQueue = resource.getCommandQueueManager().GetCommandQueue(commandQueueHandle);
        creationParams->m_fixedData.m_commandList = &(commandQueue.GetCommandList(commandLisHandle));

        //register face with model manager
        RegisterCreatedModel(Face::CreateFace((creationParams->m_fixedData), m_resource), renderResourceId);
    }

    return renderResourceId;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
bool ModelManager::HasRenderResource(size_t resource_id) const
{
    return InternalHasRenderResource(resource_id);
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void ModelManager::RegisterCreatedModel(CreatedModel model, size_t renderResourceId)
{
    std::scoped_lock<std::mutex> lock(m_mutex);
    if (!InternalHasRenderResource(renderResourceId))
    {
        ModelResourceHandle handle;
        handle.m_model = model;
        handle.m_resourceId = renderResourceId;

        m_models.push_back(handle);

        //We want to sort these here
        std::sort(begin(m_models), end(m_models), [](const ModelResourceHandle& lhs, const ModelResourceHandle& rhs) { return lhs.m_model.model->GetSortKey() < rhs.m_model.model->GetSortKey(); });
    }
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
const CreatedModel* ModelManager::GetRenderResource(size_t renderResourceId) const
{
    std::scoped_lock<std::mutex> lock(m_mutex);
    ModelsArray::const_iterator it = std::find_if(cbegin(m_models), cend(m_models), [renderResourceId](const auto& entry) { return entry.m_resourceId == renderResourceId; });
    if (it != cend(m_models))
    {
        return &(it->m_model);
    }

    return nullptr;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark This might not be needed, does this function need to actually lock
///-----------------------------------------------------------------------------
bool ModelManager::InternalHasRenderResource(size_t resourceId) const
{
    return (std::find_if(cbegin(m_models), cend(m_models), [resourceId](const auto& entry) { return entry.m_resourceId == resourceId; })) != cend(m_models);
}
