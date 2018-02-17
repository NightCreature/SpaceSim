#include "MmlLoader.h"

#include "Core/tinyxml2.h"
#include "Graphics/Model.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "Graphics/modelmanager.h"
#include "Loader/ModelLoaders/AssimpModelLoader.h"
#include <string>

const unsigned int c_ModelHash = hashString("Model");

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
CreatedModel MmlLoader::LoadModel(Resource* resource, const LoadData& loadData)
{
    if (loadData.m_fileName.empty())
    {
        return CreatedModel();
    }

    CreatedModel model;

    tinyxml2::XMLDocument document;
    if (document.LoadFile(loadData.m_fileName.c_str()) != tinyxml2::XML_NO_ERROR)
    {
        MSG_TRACE_CHANNEL("MMLLoader Error", "Failed to load %s\nWith error: %d", loadData.m_fileName.c_str(), document.ErrorID())
        return model;
    }

    const RenderResource& gameResource = RenderResourceHelper(resource).getResource();
    Material mat;
    std::string modelFileName;

    const tinyxml2::XMLElement* element;
    element = document.FirstChildElement();
    for (element = element->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
    {
        unsigned int element_hash = hashString(element->Name());

        if (element_hash == c_ModelHash)
        {
            const tinyxml2::XMLAttribute* attribute = element->FindAttribute("file_name");
            modelFileName = attribute->Value();
        }
        else if (element_hash == Material::m_hash)
        {
            mat.deserialise(resource, gameResource.getDeviceManager(), gameResource.getTextureManager(), gameResource.getLightManager(), element);
        }
    }

    if (!modelFileName.empty())
    {
        model = AssimpModelLoader::LoadModel(resource, mat, modelFileName); //This needs to change to a mesh creation function instead because no more assimp loading on its own
    }

    return model;
}