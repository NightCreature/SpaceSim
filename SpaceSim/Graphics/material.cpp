#include "Graphics/material.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "Windows.h"
#include "Graphics/texture.h"
#include "Graphics/texturemanager.h"
#include "Graphics/DeviceManager.h"
#include "Graphics/EffectCache.h"
#include "Graphics/LightManager.h"
#include "Core/Types/TypeHelpers.h"



//Material::Material() :
//m_materialCB(),
//m_techniqueHash("default"_hash),
//m_alphaBlend(false)
//{
//}

Material::Material(float shininess, const Color& ambient, const Color& specular, const Color& emissive, const Color& diffuse) :
m_techniqueHash("default"_hash)
{
    m_materialCB.ambient[3] = ambient.a();
    m_materialCB.ambient[0] = ambient.r();
    m_materialCB.ambient[1] = ambient.g();
    m_materialCB.ambient[2] = ambient.b();
    m_materialCB.diffuse[3] = diffuse.a();
    m_materialCB.diffuse[0] = diffuse.r();
    m_materialCB.diffuse[1] = diffuse.g();
    m_materialCB.diffuse[2] = diffuse.b();
    m_materialCB.emissive[3] = emissive.a();
    m_materialCB.emissive[0] = emissive.r();
    m_materialCB.emissive[1] = emissive.g();
    m_materialCB.emissive[2] = emissive.b();
    m_materialCB.specular[3] = specular.a();
    m_materialCB.specular[0] = specular.r();
    m_materialCB.specular[1] = specular.g();
    m_materialCB.specular[2] = specular.b();
    m_materialCB.shininess = shininess;
    m_alphaBlend = false;
}

Material::Material(const Material& material)
{
    m_materialCB = material.m_materialCB;
    m_texture.insert(m_texture.begin(), material.m_texture.begin(), material.m_texture.end());
    m_alphaBlend = material.m_alphaBlend;
    m_techniqueHash = material.getTechnique();
    m_effectHash = material.getEffectHash();
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void Material::deserialise( Resource* resource, DeviceManager& deviceManager, const TextureManager& textureManger, const LightManager& lightManager, const tinyxml2::XMLElement* node )
{
    UNUSEDPARAM(resource);

    node = node->FirstChildElement();
    const auto shininessHash = "Shininess"_hash;
    const auto alphaBlend = "AlphaBlend"_hash;
    unsigned int numberOfColorsDeserialised = 1;
    for(;node; node = node->NextSiblingElement())
    {
        auto elementHash = Hashing::hashString(node->Value());
        if( Color::m_hash == elementHash)
        {
            Color temp;
            switch (numberOfColorsDeserialised)
            {
            case 1:
                temp.deserialise(node);
                m_materialCB.ambient[0] = temp.r();
                m_materialCB.ambient[1] = temp.g();
                m_materialCB.ambient[2] = temp.b();
                m_materialCB.ambient[3] = temp.a();
                break;
            case 2:
                temp.deserialise(node);
                m_materialCB.diffuse[0] = temp.r();
                m_materialCB.diffuse[1] = temp.g();
                m_materialCB.diffuse[2] = temp.b();
                m_materialCB.diffuse[3] = temp.a();
                break;
            case 3:
                temp.deserialise(node);
                m_materialCB.specular[0] = temp.r();
                m_materialCB.specular[1] = temp.g();
                m_materialCB.specular[2] = temp.b();
                m_materialCB.specular[3] = temp.a();
                break;
            case 4:
                temp.deserialise(node);
                m_materialCB.emissive[0] = temp.r();
                m_materialCB.emissive[1] = temp.g();
                m_materialCB.emissive[2] = temp.b();
                m_materialCB.emissive[3] = temp.a();
                break;
            }
            
            ++numberOfColorsDeserialised;
        }
        else if( shininessHash == elementHash)
        {
            const tinyxml2::XMLAttribute* attribute = node->FindAttribute("value");
            if (attribute)
            {
                m_materialCB.shininess = attribute->FloatValue();
            }
        }
        else if (Texture::m_hash == elementHash)
        {
            TextureManager& tm = (TextureManager&)textureManger;
            m_texture.push_back(tm.deserialise(deviceManager, node));
        }
        else if (Effect::m_hash == elementHash)
        {
            const tinyxml2::XMLAttribute* attribute = node->FindAttribute("file_name");
            if (attribute)
            {
                m_effectHash = HashString( getResourceNameFromFileName(attribute->Value()) );
            }
            attribute = node->FindAttribute("technique_name");
            if (attribute)
            {
                m_techniqueHash = HashString(attribute->Value());
            }
            //m_effect.deserialise(deviceManager, node);
        }
        else if (alphaBlend == elementHash)
        {
            const tinyxml2::XMLAttribute* attribute = node->FindAttribute("enabled");
            if (attribute != nullptr)
            {
                m_alphaBlend = attribute->BoolValue();
            }
        }
        //Dont need this since we reflect on the shader now
        //else if ("ShaderParameterData"_hash == elementHash)
        //{
        //    //Deserialise the data we need here, we can write to this data field
        //    constexpr size_t typeHashes[] = { "WVPBufferContent"_hash , "MaterialContent"_hash , "PerFrameConstants"_hash }; //This should probably live with the shader parameters
        //    for (const auto* childNode = node->FirstChildElement(); childNode != nullptr; childNode = static_cast<const tinyxml2::XMLElement*>(childNode->NextSibling()))
        //    {
        //        if (hashString(childNode->Value()) == "ShaderParameterData"_hash)
        //        {
        //            ShaderParameterData data;

        //            auto* attribute = childNode->FindAttribute("name");
        //            if (attribute != nullptr)
        //            {
        //                data.m_nameHash = hashString(attribute->Value());
        //            }
        //            attribute = childNode->FindAttribute("type");
        //            if (attribute != nullptr)
        //            {
        //                auto typeHash = hashString(attribute->Value());

        //            }
        //        }
        //    }
        //}
    }

    UNUSEDPARAM(lightManager);
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
Material::MaterialParameters Material::GetMaterialParameters(const tinyxml2::XMLElement* childElement)
{
    MaterialParameters returnVal;
    childElement = childElement->FirstChildElement();
    const auto shininessHash = "Shininess"_hash;
    const auto alphaBlend = "AlphaBlend"_hash;
    unsigned int numberOfColorsDeserialised = 1;
    for (; childElement; childElement = childElement->NextSiblingElement())
    {
        auto elementHash = Hashing::hashString(childElement->Value());
        if (Color::m_hash == elementHash)
        {
            Color temp;
            switch (numberOfColorsDeserialised)
            {
            case 1:
                temp.deserialise(childElement);
                returnVal.m_materialContent.ambient[0] = temp.r();
                returnVal.m_materialContent.ambient[1] = temp.g();
                returnVal.m_materialContent.ambient[2] = temp.b();
                returnVal.m_materialContent.ambient[3] = temp.a();
                break;
            case 2:
                temp.deserialise(childElement);
                returnVal.m_materialContent.diffuse[0] = temp.r();
                returnVal.m_materialContent.diffuse[1] = temp.g();
                returnVal.m_materialContent.diffuse[2] = temp.b();
                returnVal.m_materialContent.diffuse[3] = temp.a();
                break;
            case 3:
                temp.deserialise(childElement);
                returnVal.m_materialContent.specular[0] = temp.r();
                returnVal.m_materialContent.specular[1] = temp.g();
                returnVal.m_materialContent.specular[2] = temp.b();
                returnVal.m_materialContent.specular[3] = temp.a();
                break;
            case 4:
                temp.deserialise(childElement);
                returnVal.m_materialContent.emissive[0] = temp.r();
                returnVal.m_materialContent.emissive[1] = temp.g();
                returnVal.m_materialContent.emissive[2] = temp.b();
                returnVal.m_materialContent.emissive[3] = temp.a();
                break;
            }

            ++numberOfColorsDeserialised;
        }
        else if (shininessHash == elementHash)
        {
            const tinyxml2::XMLAttribute* attribute = childElement->FindAttribute("value");
            if (attribute)
            {
                returnVal.m_materialContent.shininess = attribute->FloatValue();
            }
        }
        else if (Texture::m_hash == elementHash)
        {
            const char* fileName = childElement->Attribute("file_name");
            if (fileName)
            {
                Material::TextureSlotMapping::TextureSlot textureSlot = Material::TextureSlotMapping::Diffuse0;
                const tinyxml2::XMLAttribute* attribute = childElement->FindAttribute("texture_slot");
                if (attribute != nullptr)
                {
                    textureSlot = static_cast<Material::TextureSlotMapping::TextureSlot>(attribute->UnsignedValue());
                }

                stringCopy(returnVal.m_textureNames[textureSlot], fileName);
            }
        }
        else if (Effect::m_hash == elementHash)
        {
            const tinyxml2::XMLAttribute* attribute = childElement->FindAttribute("file_name");
            if (attribute)
            {
                returnVal.m_effectHash = HashString(getResourceNameFromFileName(attribute->Value()) );
            }
            returnVal.m_techniqueHash = "default"_hashstring; //In case we dont have an attached technique name
            attribute = childElement->FindAttribute("technique_name");
            if (attribute)
            {
                returnVal.m_techniqueHash = HashString(attribute->Value());
            }
            //m_effect.deserialise(deviceManager, childElement);
        }
        else if (alphaBlend == elementHash)
        {
            const tinyxml2::XMLAttribute* attribute = childElement->FindAttribute("enabled");
            if (attribute != nullptr)
            {
                returnVal.m_alphaBlend = attribute->BoolValue();
            }
        }
    }

    return returnVal;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void Material::Prepare(const EffectCache& effectCache)
{
    //With bindless we have to do nothing here, or we could bind the textures here
    UNUSEDPARAM(effectCache);
}

void Material::OnDebugImgui(const std::string_view& name)
{
    m_materialCB.OnDebugImgui(name);
}
