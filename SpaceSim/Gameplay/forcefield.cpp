#include "Gameplay/forcefield.h"
#include "Math/matrix44.h"
#include "Core/Settings/SettingsManager.h"
#include "Graphics/texturemanager.h"
#include "Windows.h"

#include "Core/MessageSystem/MessageQueue.h"
#include "Core/MessageSystem/GameMessages.h"
#include "Core/MessageSystem/RenderMessages.h"

#include <iostream>


#include "Gameplay/box.h"



ForceField::ForceField(Resource* resource, const Vector3& position):
GameObject(resource)
{
	//position is the middle of a corridor
	m_position = position;
	m_texturespeed = 0.0f;
	m_active = true;

    m_name = "forcefield";
    m_nameHash = hashString(m_name);
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void ForceField::initialise(const ShaderInstance& shaderInstance, bool changeWindingOrder)
{
	UNUSEDPARAM(shaderInstance);
    Face::CreationParams params;
//    params.shaderInstance = &shaderInstance;
//    params.resource = m_resource;
    params.fillvalue = 0.0f;
    params.fillx = true;
    params.changeWindingOrder = changeWindingOrder;
    params.m_materialParameters = m_materialParameters;
    //CreatedModel face = Face::CreateFace(params);
    //m_drawableObject = face.model;
    //m_active = true;

    DECLAREANDCREATERESOURCEMESSAGE(createPlaneModel, Face::CreationParams);
    createPlaneModel.SetData(params);
    createPlaneModel.SetGameObjectId(static_cast<size_t>(m_nameHash)); //Not super but should work for now
    GameResourceHelper(m_resource).getWriteableResource().m_messageQueues->getUpdateMessageQueue()->addMessage(createPlaneModel); //Init isnt done here because we are waiting for a response from the render thread

    //Super::initialise(shaderInstance);
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
const ShaderInstance ForceField::deserialise( const tinyxml2::XMLElement* element)
{
    ShaderInstance shaderInstance;
    
    const tinyxml2::XMLAttribute* attribute = element->FindAttribute("name");
    if (attribute != nullptr)
    {
        m_name = attribute->Value();
        m_nameHash = hashString(m_name);
    }

    for (element = element->FirstChildElement(); element != 0; element = element->NextSiblingElement())
    {
        auto typeHash = hashString(element->Value());
        if (Material::m_hash == typeHash)
        {
            MSG_TRACE_CHANNEL("REFACTOR", "SEND create material message to render system");
            //shaderInstance.getMaterial().deserialise(m_resource, getResource().getDeviceManager(), getResource().getTextureManager(), getResource().getLightManager(), element);
            //shaderInstance.getMaterial().setBlendState(true);
            m_materialParameters = Material::GetMaterialParameters(element);
        }
        else if (Vector3::m_hash == typeHash)
        {
            m_position.deserialise(element);
            translate(m_world, m_position.x(), m_position.y(), m_position.z());
        }
    }

    return shaderInstance;
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void ForceField::update( RenderInstanceTree& renderInstances, float elapsedTime, const Input& input )
{
    //React to input before doing the draw update below
    if (m_active)
    {
        GameObject::update(renderInstances, elapsedTime, input);
        if (m_texturespeed < 1.0f)
        {
        	m_texturespeed += 0.01f;
        }
        else
        {
            m_texturespeed = 0.0f;
        }

        MessageSystem::RenderInformation renderInfo;
        MessageSystem::RenderInformation::RenderInfo data;
        data.m_renderObjectid = m_renderHandle;
        data.m_gameobjectid = m_nameHash;
        data.m_world = m_world;
        data.m_name = m_name.c_str();
        renderInfo.SetData(data);
        m_resource->m_messageQueues->getUpdateMessageQueue()->addMessage(renderInfo);
    }
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void ForceField::handleMessage( const MessageSystem::Message& msg )
{
    UNUSEDPARAM(msg);
    //const ActivationMessage& activateMsg = (const ActivationMessage&)msg;
    //setActive(activateMsg.shouldActivate());
    if (msg.getMessageId() == MESSAGE_ID(CreatedRenderResourceMessage))
    {
        const MessageSystem::CreatedRenderResourceMessage& renderResourceMsg = static_cast<const MessageSystem::CreatedRenderResourceMessage&>(msg);
        renderResourceMsg.GetData();
        m_renderHandle = renderResourceMsg.GetData()->m_renderResourceHandle;
        //Store the render object reference we get back and the things it can do
        m_initialisationDone = true;
        m_active = true;
    }
}
