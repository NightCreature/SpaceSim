#include "Graphics/material.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "Windows.h"
#include "Graphics/texture.h"
#include "Graphics/texturemanager.h"
#include "Graphics/DeviceManager.h"
#include "Graphics/EffectCache.h"
#include "Graphics/LightManager.h"
#include "Core/Types/TypeHelpers.h"

HASH_ELEMENT_IMPLEMENTATION(Material)

Material::Material() :
m_materialCB(),
m_alphaBlend(false),
m_techniqueHash(hashString("default"))
{
}

Material::Material(float shininess, const Color& ambient, const Color& specular, const Color& emissive, const Color& diffuse) :
m_techniqueHash(hashString("default"))
{
    m_materialCB.m_ambient[3] = ambient.a();
    m_materialCB.m_ambient[0] = ambient.r();
    m_materialCB.m_ambient[1] = ambient.g();
    m_materialCB.m_ambient[2] = ambient.b();
    m_materialCB.m_diffuse[3] = diffuse.a();
    m_materialCB.m_diffuse[0] = diffuse.r();
    m_materialCB.m_diffuse[1] = diffuse.g();
    m_materialCB.m_diffuse[2] = diffuse.b();
    m_materialCB.m_emissive[3] = emissive.a();
    m_materialCB.m_emissive[0] = emissive.r();
    m_materialCB.m_emissive[1] = emissive.g();
    m_materialCB.m_emissive[2] = emissive.b();
    m_materialCB.m_specular[3] = specular.a();
    m_materialCB.m_specular[0] = specular.r();
    m_materialCB.m_specular[1] = specular.g();
    m_materialCB.m_specular[2] = specular.b();
    m_materialCB.m_shininess = shininess;
    m_alphaBlend = false;
}

Material::Material(const Material& material)
{
    m_effect = material.m_effect;
    m_materialCB = material.m_materialCB;
    m_texture.insert(m_texture.begin(), material.m_texture.begin(), material.m_texture.end());
    m_alphaBlend = material.m_alphaBlend;
    m_techniqueHash = material.getTechnique();
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void Material::deserialise( Resource* resource, const DeviceManager& deviceManager, const TextureManager& textureManger, const LightManager& lightManager, const tinyxml2::XMLElement* node )
{
    node = node->FirstChildElement();
    static unsigned int shininessHash = hashString("Shininess");
    static unsigned int alphaBlend = hashString("AlphaBlend");
    unsigned int numberOfColorsDeserialised = 1;
    for(;node; node = node->NextSiblingElement())
    {
        unsigned int elementHash = hashString(node->Value());
        if( Color::m_hash == elementHash)
        {
            Color temp;
            switch (numberOfColorsDeserialised)
            {
            case 1:
                temp.deserialise(node);
                m_materialCB.m_ambient[0] = temp.r();
                m_materialCB.m_ambient[1] = temp.g();
                m_materialCB.m_ambient[2] = temp.b();
                m_materialCB.m_ambient[3] = temp.a();
                break;
            case 2:
                temp.deserialise(node);
                m_materialCB.m_diffuse[0] = temp.r();
                m_materialCB.m_diffuse[1] = temp.g();
                m_materialCB.m_diffuse[2] = temp.b();
                m_materialCB.m_diffuse[3] = temp.a();
                break;
            case 3:
                temp.deserialise(node);
                m_materialCB.m_specular[0] = temp.r();
                m_materialCB.m_specular[1] = temp.g();
                m_materialCB.m_specular[2] = temp.b();
                m_materialCB.m_specular[3] = temp.a();
                break;
            case 4:
                temp.deserialise(node);
                m_materialCB.m_emissive[0] = temp.r();
                m_materialCB.m_emissive[1] = temp.g();
                m_materialCB.m_emissive[2] = temp.b();
                m_materialCB.m_emissive[3] = temp.a();
                break;
            }
            
            ++numberOfColorsDeserialised;
        }
        else if( shininessHash == elementHash)
        {
            const tinyxml2::XMLAttribute* attribute = node->FindAttribute("value");
            if (attribute)
            {
                m_materialCB.m_shininess = attribute->FloatValue();
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
                GameResourceHelper helper(resource);
                m_effect = const_cast<Effect*>(helper.getWritableGameResource().getEffectCache().createEffect(resource, attribute->Value()));
            }
            attribute = node->FindAttribute("technique_name");
            if (attribute)
            {
                m_techniqueHash = hashString(attribute->Value());
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
    }

    UNUSEDPARAM(lightManager);
}

