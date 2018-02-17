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
size_t ModelManager::LoadModel( void* data )
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

        CreatedModel createdModel;
        if (strICmp(extension, "dat") || strICmp(extension, "xml"))
        {
            XMLModelLoader loader;
            createdModel = loader.LoadModel(m_resource, loadData);
            //m_loadedModels.insert(GeometryTreePair(fileNameHash, model));
        }
        else if (strICmp(extension, "mml"))
        {
            //This format is a mixture between an xml and a attached model file
            MmlLoader loader;
            createdModel = loader.LoadModel(m_resource, loadData);
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
size_t ModelManager::AddFace(void* data)
{
    auto creationParams = static_cast<MessageSystem::CreateRenderResource<Face::CreationParams>::ResourceData<Face::CreationParams>*>(data);
    size_t renderResourceId = HASH_BINARY(creationParams);
    if (!HasRenderResource(renderResourceId))
    {
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
    return (std::find_if(cbegin(m_models), cend(m_models), [resource_id](const auto& entry) { return entry.m_resourceId == resource_id; })) != cend(m_models);

}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
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

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
const CreatedModel* ModelManager::GetRenderResource(size_t renderResourceId) const
{
    ModelsArray::const_iterator it = std::find_if(cbegin(m_models), cend(m_models), [renderResourceId](const auto& entry) { return entry.m_resourceId == renderResourceId; });
    if (it != cend(m_models))
    {
        return &(it->m_model);
    }

    return nullptr;
}
