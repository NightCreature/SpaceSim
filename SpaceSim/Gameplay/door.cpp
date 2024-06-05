#include "Gameplay/door.h"
#include "Math/matrix44.h"
#include "Core/Settings/SettingsManager.h"
#include "Graphics/texturemanager.h"
#include "Graphics/DeviceManager.h"

#include "Core/MessageSystem/MessageQueue.h"
#include "Core/MessageSystem/GameMessages.h"
#include "Core/MessageSystem/RenderMessages.h"
#include "Core/Resource/GameResource.h"

#include <iostream>
#include <D3D11.h>




Door::Door(Resource* resource, const Vector3& position):
GameObject(resource)
{
    //position is the middle of a corridor
    m_position = position;
    m_move = 0.0f;
    m_plus = true;
    m_active = true;
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void Door::initialise(bool changeWindingOrder)
{
    Face::CreationParams params;
//    params.shaderInstance = &shaderInstance;
//    params.resource = m_resource;
    params.fillvalue = 0.0f;
    params.fillx = true;
    params.changeWindingOrder = changeWindingOrder;
    params.m_materialParameters = m_materialParameters;
    params.m_name = m_name;
    //CreatedModel face = Face::CreateFace(params);
    //m_drawableObject = face.model;
    //m_active = true;

    //Super::initialise(shaderInstance);

    DECLAREANDCREATERESOURCEMESSAGE(createPlaneModel, Face::CreationParams);
    createPlaneModel.SetData(params);
    createPlaneModel.SetGameObjectId(static_cast<size_t>(m_nameHash)); //Not super but should work for now
    GameResourceHelper(m_resource).getWriteableResource().m_messageQueues->getUpdateMessageQueue()->addMessage(createPlaneModel); //Init isnt done here because we are waiting for a response from the render thread
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void Door::deserialise( const tinyxml2::XMLElement* element)
{
    const tinyxml2::XMLAttribute* attribute = element->FindAttribute("name");
    if (attribute != nullptr)
    {
        m_name = attribute->Value();
        m_nameHash = Hashing::hashString(m_name);
    }

    for (element = element->FirstChildElement(); element != 0; element = element->NextSiblingElement())
    {
        auto typeHash = Hashing::hashString(element->Value());
        if (Material::m_hash == typeHash)
        {
            MSG_TRACE_CHANNEL("REFACTOR", "SEND create material message to render system");
            m_materialParameters = Material::GetMaterialParameters(element);
        }
        else if (Vector3::m_hash == typeHash)
        {
            m_position.deserialise(element);
            translate(m_world, m_position.x(), m_position.y(), m_position.z());
        }
    }
}

///-------------------------------------------------------------------------
// @brief
///-------------------------------------------------------------------------
void Door::update( float elapsedTime, const Input& input )
{
    if (m_initialisationDone)
    {
        //move
        Matrix44 transform;
        if (!m_active)
        {
            if (m_move < 50.0f)
                m_move += 10.0f * elapsedTime;

            translate(transform, 0.0f, 0.0f, m_move);
            
        }
        else
        {
            if (m_move > 0.0f)
                m_move -= 10.0f * elapsedTime;
            translate(transform, 0.0f, 0.0f, m_move);
        }

        m_world = m_world * transform;
        //Super::update(renderInstances, elapsedTime, input);
#ifdef _DEBUG
    //renderInstances.back()->m_name = L"Door";
#endif

        UNUSEDPARAM(input);

        MessageSystem::RenderInformation renderInfo;
        MessageSystem::RenderInformation::RenderInfo data;
        data.m_renderObjectid = m_renderHandle;
        data.m_gameobjectid = m_nameHash;
        data.m_world = m_world;
        data.m_name = m_name.c_str();
        data.m_shouldRender = true;
        renderInfo.SetData(data);
        m_resource->m_messageQueues->getUpdateMessageQueue()->addMessage(renderInfo);
    }
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void Door::handleMessage( const MessageSystem::Message& msg )
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
