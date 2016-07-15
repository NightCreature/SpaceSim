#include "Graphics/modelmanager.h"
#include "assert.h"
#include <iostream>
#include "Graphics/Model.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "Loader/ModelLoaders/AssimpModelLoader.h"
#include "Loader/ModelLoaders/XMLModelLoader.h"
#include "Loader/ModelLoaders/MmlLoader.h"

ModelManager::~ModelManager()
{
    cleanup();
}

void ModelManager::cleanup()
{
    for (GeometryTreeIterator it = m_loadedModels.begin(); it != m_loadedModels.end(); ++it)
    {
        if (it->second != nullptr)
        {
            Model* model = it->second;
            delete model;
            it->second = nullptr;
        }
    }
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
    
    return model;
}
