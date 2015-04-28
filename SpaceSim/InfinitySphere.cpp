#include "InfinitySphere.h"
#include "StringHelperFunctions.h"
#include "CameraManager.h"
#include "ModelManager.h"

HASH_ELEMENT_IMPLEMENTATION(InfinitySphere);

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
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
            shaderInstance.getMaterial().deserialise(m_resource, getGameResource().getDeviceManager(), getGameResource().getTextureManager(), getGameResource().getLightManager(), childElement);
        }
        else if (childElementHash == modelHash)
        {
            const tinyxml2::XMLAttribute* attribute = childElement->FindAttribute("file");
            if (attribute != nullptr)
            {
                //Heavily relies on the shader instance existing before we load the model, might be better to put the model construction in initialise instead
                m_drawableObject = getWriteableGameResource().getModelManager().LoadModel(m_resource, shaderInstance, attribute->Value());
            }
        }
        else if (Vector3::m_hash == childElementHash)
        {
            m_scale.deserialise(childElement);
        }
    }

    return shaderInstance;
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void InfinitySphere::initialise(const ShaderInstance& shaderInstance)
{
    //m_drawableObject->initialise(shaderInstance);
    Super::initialise(shaderInstance);
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void InfinitySphere::update(RenderInstanceTree& renderInstances, float elapsedTime, const Input& input)
{
    const Camera* cam = getGameResource().getCameraManager().getCamera("global");
    if (cam != nullptr)
    {
        scale(m_world, m_scale.x(), m_scale.y(), m_scale.z());
        Matrix44 temp;
        translate(temp, cam->getEye());
        m_world = m_world * temp;
    }

    Super::update(renderInstances, elapsedTime, input);
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void InfinitySphere::handleMessage(const Message& msg)
{
    UNUSEDPARAM(msg);
}
