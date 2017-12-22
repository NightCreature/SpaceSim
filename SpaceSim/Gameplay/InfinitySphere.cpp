#include "Gameplay/InfinitySphere.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "Graphics/CameraManager.h"
#include "Graphics/ModelManager.h"

HASH_ELEMENT_IMPLEMENTATION(InfinitySphere);

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
const ShaderInstance InfinitySphere::deserialise(const tinyxml2::XMLElement* node)
{
    m_name = "skydome";
    ShaderInstance shaderInstance;

    const unsigned int modelHash = hashString("Model");
    for (const tinyxml2::XMLElement* childElement = node->FirstChildElement(); childElement; childElement = childElement->NextSiblingElement())
    {
        unsigned int childElementHash = hashString(childElement->Value());
        if (childElementHash == Material::m_hash)
        {
            MSG_TRACE_CHANNEL("REFACTOR", "SEND create material message to render system");
            //shaderInstance.getMaterial().deserialise(m_resource, getResource().getDeviceManager(), getResource().getTextureManager(), getResource().getLightManager(), childElement);
        }
        else if (childElementHash == modelHash)
        {
            const tinyxml2::XMLAttribute* attribute = childElement->FindAttribute("file");
            if (attribute != nullptr)
            {
                MSG_TRACE_CHANNEL("REFACTOR", "SEND create material message to render system");
                //Heavily relies on the shader instance existing before we load the model, might be better to put the model construction in initialise instead
                //m_drawableObject = getWriteableResource().getModelManager().LoadModel(m_resource, shaderInstance, attribute->Value());
                //m_drawableObject->getMeshData()[0]->getShaderInstance().getMaterial().addTextureReference(Material::TextureSlotMapping(hashString("skybox_small.dds"), Material::TextureSlotMapping::Diffuse0)); //Hacky
                //m_drawableObject->setDirty();
            }
        }
        else if (Vector3::m_hash == childElementHash)
        {
            m_scale.deserialise(childElement);
        }
    }

    return shaderInstance;
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void InfinitySphere::initialise(const ShaderInstance& shaderInstance)
{
    //m_drawableObject->initialise(shaderInstance);
    Super::initialise(shaderInstance);
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void InfinitySphere::update(RenderInstanceTree& renderInstances, float elapsedTime, const Input& input)
{
    MSG_TRACE_CHANNEL("REFACTOR", "SEND Cameraid this should be centered arround");
    //const Camera* cam = getResource().getCameraManager().getCamera("global");
    //if (cam != nullptr)
    //{
    //    scale(m_world, m_scale.x(), m_scale.y(), m_scale.z());
    //    Matrix44 temp;
    //    translate(temp, cam->getEye());
    //    m_world = m_world * temp;
    //}

    Super::update(renderInstances, elapsedTime, input);
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void InfinitySphere::handleMessage(const MessageSystem::Message& msg)
{
    UNUSEDPARAM(msg);
}
