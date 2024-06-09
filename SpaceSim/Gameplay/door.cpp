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
#include "imgui.h"




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
void Door::DeserialiseInternal( const tinyxml2::XMLElement* element)
{
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

void Door::OnDebugImguiInternal()
{
    ImGui::CollapsingHeader("Material");
    ImGui::InputFloat4("Ambient", m_materialParameters.m_materialContent.ambient.GetDataPtr());
    ImGui::InputFloat4("Diffuse", m_materialParameters.m_materialContent.diffuse.GetDataPtr());
    ImGui::InputFloat4("Specular", m_materialParameters.m_materialContent.specular.GetDataPtr());
    ImGui::InputFloat4("Emissive", m_materialParameters.m_materialContent.emissive.GetDataPtr());
    ImGui::InputFloat("Shininess", &m_materialParameters.m_materialContent.shininess);
    ImGui::Checkbox("Alpha Enabled", &m_materialParameters.m_alphaBlend);
}
