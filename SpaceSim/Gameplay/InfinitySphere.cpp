#include "Gameplay/InfinitySphere.h"
#include "Core/MessageSystem/MessageQueue.h"
#include "Core/Resource/GameResource.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "Graphics/CameraManager.h"
#include "Graphics/ModelManager.h"
#include "Loader/ModelLoaders/ModelLoader.h"
#include <string>



///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void InfinitySphere::deserialise(const tinyxml2::XMLElement* node)
{
    m_name = "skydome";

    for (const tinyxml2::XMLElement* childElement = node->FirstChildElement(); childElement; childElement = childElement->NextSiblingElement())
    {
        auto childElementHash = hashString(childElement->Value());
        if (childElementHash == Material::m_hash)
        {
            MSG_TRACE_CHANNEL("REFACTOR", "SEND create material message to render system");
            //shaderInstance.getMaterial().deserialise(m_resource, getResource().getDeviceManager(), getResource().getTextureManager(), getResource().getLightManager(), childElement);
        }
        else if (childElementHash == "Model"_hash)
        {
            const tinyxml2::XMLAttribute* attribute = childElement->FindAttribute("file");
            if (attribute != nullptr)
            {
                MSG_TRACE_CHANNEL("REFACTOR", "SEND create material message to render system");
                //Heavily relies on the shader instance existing before we load the model, might be better to put the model construction in initialise instead
                //m_drawableObject = getWriteableResource().getModelManager().LoadModel(m_resource, shaderInstance, attribute->Value());
                //m_drawableObject->getMeshData()[0]->getShaderInstance().getMaterial().addTextureReference(Material::TextureSlotMapping(hashString("skybox_small.dds"), Material::TextureSlotMapping::Diffuse0)); //Hacky
                //m_drawableObject->setDirty();
                std::string_view modelFileName(attribute->Value());
                LoadModelResource modelResource;
                memcpy(&modelResource.m_fileName, modelFileName.data(), modelFileName.size());
                modelResource.m_fileName[modelFileName.size()] = 0;

                auto resource = GameResourceHelper(m_resource).getWriteableResource();
                DECLAREANDCREATERESOURCEMESSAGE(createModel, LoadModelResource);
                createModel.SetData(modelResource);
                createModel.SetGameObjectId(static_cast<size_t>(m_nameHash)); //Not super but should work for now

                resource.m_messageQueues->getUpdateMessageQueue()->addMessage(createModel);

                Super::initialise();

            }
        }
        else if (Vector3::m_hash == childElementHash)
        {
            m_scale.deserialise(childElement);
        }
    }
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void InfinitySphere::initialise()
{
    //m_drawableObject->initialise(shaderInstance);
    Super::initialise();
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void InfinitySphere::update(float elapsedTime, const Input& input)
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

    Super::update(elapsedTime, input);
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void InfinitySphere::handleMessage(const MessageSystem::Message& msg)
{
    UNUSEDPARAM(msg);
}
