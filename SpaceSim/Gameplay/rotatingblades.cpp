#include "Gameplay/rotatingblades.h"
#include "Math/matrix44.h"
#include "Graphics/texturemanager.h"
#include "Windows.h"

#include "Core/MessageSystem/MessageQueue.h"
#include "Core/MessageSystem/GameMessages.h"
#include "Core/MessageSystem/RenderMessages.h"

#include <iostream>
#include "imgui.h"




RotatingBlades::RotatingBlades(Resource* resource, const Vector3& position) :
GameObject(resource)
{
	//position is the middle of a corridor
	m_position = position;
	m_angle = 0.0f;
	m_active = true;
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void RotatingBlades::deserialise( const tinyxml2::XMLElement* element)
{
    const tinyxml2::XMLAttribute* attribute = element->FindAttribute("name");
    if (attribute != nullptr)
    {
        m_name = attribute->Value();
    }

    for (element = element->FirstChildElement(); element != 0; element = element->NextSiblingElement())
    {
        auto typeHash = Hashing::hashString(element->Value());
        if (Material::m_hash == typeHash)
        {
            //This all needs to be a message to the renderer to create a render object
            MSG_TRACE_CHANNEL("REFACTOR", "CREATE RENDER RESOURCE HERE THROUGH A MESSAGE");
            //shaderInstance.getMaterial().deserialise(m_resource, getResource().getDeviceManager(), getResource().getTextureManager(), getResource().getLightManager(), element);
            //shaderInstance.getMaterial().setBlendState(true);
            m_materialParameters = Material::GetMaterialParameters(element);
        }
        else if (Vector3::m_hash == typeHash)
        {
            //m_position.deserialise(element);
            //translate(m_world, m_position.x(), m_position.y(), m_position.z());
            //Matrix44 transform;
            //translate(transform, 0.0f, 0.0f, -25.0f);
            //Matrix44 world = m_world;
            //shaderInstance.setWorld(m_world * transform);
            //Matrix44 transform2;
            //rotate(transform2, Vector3::yAxis(), 90);
            //Matrix44 translateX;
            //translate(translateX, 25.0f, 0.0f, 0.0f);
            //Matrix44 temp = transform2 * translateX * m_world;
            //m_rotatingblades2.setWorld(temp);
        }
    }
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void RotatingBlades::update( float elapsedTime, const Input& input )
{
    m_angle += 25.0f*elapsedTime;

    if (m_active)
    {
        //m_rotatingblades1.update( renderInstances, elapsedTime, m_world );
//#ifdef _DEBUG
//        renderInstances.back()->m_name = L"Rotating blades 1";
//#endif
        //m_rotatingblades2.update(renderInstances, elapsedTime, m_world, "Rotating blades 2");

        //glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0f);
        //glDepthMask(false);
        //TextureManager& tm = TextureManager::getInstance();

        //.glActiveTexture(GL_TEXTURE0);
        //glEnable(GL_TEXTURE_2D);
        //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        //tm.activateTexture(SpaceSim::getInstance().getstd::stringSetting("blades"));

        //.glActiveTexture(GL_TEXTURE1);
        //glEnable(GL_TEXTURE_2D);
        //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        //tm.activateTexture(SpaceSim::getInstance().getstd::stringSetting("bladesalpha"));

        //glDisable(GL_CULL_FACE);
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //glEnable(GL_TEXTURE_2D);

        //glPushMatrix();
        //glTranslatef(m_position.x(), m_position.y(), m_position.z());
        //glRotatef(m_angle, 0.0f, 0.1f, 0.0f);
        //m_rotatingblades1.draw();
        //m_rotatingblades2.draw();
        //glPopMatrix();
        //glEnable(GL_CULL_FACE);
        //glDisable(GL_BLEND);
        //.glClientActiveTexture(GL_TEXTURE1);
        //glDisable(GL_TEXTURE_2D);
        //.glClientActiveTexture(GL_TEXTURE0);
        //glDisable(GL_TEXTURE_2D);
        //glDepthMask(true);
        //glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0f);
    }

    //Minimap
    //glPointSize(4.0f);
    //glColor4fv(Color::cyan().rgba());
    //glBegin(GL_POintS);
    //(m_position.x(), m_position.y(), m_position.z() + 25.0f);
    //glEnd();

    UNUSEDPARAM(input);
    UNUSEDPARAM(elapsedTime);
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void RotatingBlades::handleMessage( const MessageSystem::Message& msg )
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
    }
}

void RotatingBlades::OnDebugImguiInternal()
{
    ImGui::CollapsingHeader("Material");
    ImGui::InputFloat4("Ambient", m_materialParameters.m_materialContent.ambient.GetDataPtr());
    ImGui::InputFloat4("Diffuse", m_materialParameters.m_materialContent.diffuse.GetDataPtr());
    ImGui::InputFloat4("Specular", m_materialParameters.m_materialContent.specular.GetDataPtr());
    ImGui::InputFloat4("Emissive", m_materialParameters.m_materialContent.emissive.GetDataPtr());
    ImGui::InputFloat("Shininess", &m_materialParameters.m_materialContent.shininess);
    ImGui::Checkbox("Alpha Enabled", &m_materialParameters.m_alphaBlend);
}
