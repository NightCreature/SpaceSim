#include "MmlLoader.h"

#include "Core/tinyxml2.h"
#include "Graphics/Model.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "Graphics/modelmanager.h"
#include <string>

const unsigned int c_ModelHash = hashString("Model");

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
Model* MmlLoader::LoadModel(Resource* resource, const ShaderInstance& shaderInstance, const std::string& fileName)
{
    Model* model = nullptr;
    if (fileName.empty())
    {
        return model;
    }

    ShaderInstance& shaderInstance_ref = const_cast<ShaderInstance&>(shaderInstance);

    tinyxml2::XMLDocument document;
    if (document.LoadFile(fileName.c_str()) != tinyxml2::XML_NO_ERROR)
    {
        MSG_TRACE_CHANNEL("MMLLoader Error", "Failed to load %s\nWith error: %d", fileName.c_str(), document.ErrorID())
        return model;
    }

    const RenderResource& gameResource = RenderResourceHelper(resource).getResource();

    const tinyxml2::XMLElement* element;
    element = document.FirstChildElement();
    for (element = element->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
    {
        unsigned int element_hash = hashString(element->Name());

        if (element_hash == c_ModelHash)
        {
            const tinyxml2::XMLAttribute* attribute = element->FindAttribute("file_name");
            std::string modelFileName = attribute->Value();
            model = gameResource.getModelManager().LoadModel(resource, shaderInstance_ref, modelFileName);
        }
        else if (element_hash == Material::m_hash)
        {
            Material mat;
            mat.deserialise(resource, gameResource.getDeviceManager(), gameResource.getTextureManager(), gameResource.getLightManager(), element);
            shaderInstance_ref.setMaterial(mat);
        }
    }

    return model;
}