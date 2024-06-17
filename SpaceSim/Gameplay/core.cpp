#include "Gameplay/core.h"
#include "Graphics/DeviceManager.h"
#include "Core/Settings/SettingsManager.h"
#include "Graphics/ModelManager.h"
#include "Graphics/texturemanager.h"
#include <iostream>
#include "Core/Resource/renderResource.h"
#include "../Graphics/material.h"
#include "../Math/vector3.h"
#include "imgui.h"




///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
Core::Core(Resource* resource) :
GameObject(resource), m_position(), m_radius(5.0f), m_slices(10), m_stacks(10), m_ffstrength(10)
{

}

Core::Core(Resource* resource, const Vector3& position, float radius, int slices, int stacks, int ffstrength) :
GameObject(resource), m_position(position), m_radius(radius), m_slices(slices), m_stacks(stacks), m_ffstrength(ffstrength)
{
	//m_core = gluNewQuadric();
	//m_coreforcefield = gluNewQuadric();
	//gluQuadricOrientation(m_core, GLU_OUTSIDE);
	//gluQuadricOrientation(m_coreforcefield, GLU_OUTSIDE);
	//gluQuadricTexture(m_core, GLU_TRUE);
	//gluQuadricTexture(m_coreforcefield, GLU_TRUE);
	m_ffangle = m_angle = 0.0f;
	m_hit = false;
	m_timepassed = 0.0f;
	m_active = true;

    //Should be moved to the drawable that is created here
    //if (m_active)
    //{
    //    m_bbox.setMin(Vector3(m_position.x() - m_radius, m_position.y() - m_radius, m_position.z() - m_radius));
    //    m_bbox.setMax(Vector3(m_position.x() + m_radius, m_position.y() + m_radius, m_position.z() + m_radius));
    //}
    //else
    //{
    //    m_bbox = Bbox(Vector3::zero(), Vector3::zero());
    //}
    //return m_bbox;
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void Core::initialise()
{
    //m_drawableObject->dontCleanupGeometry();
    m_position = Vector3(525, -75, 175);
    m_world = scale(m_radius, m_radius, m_radius) * translate(m_position);
    //m_drawableObject->setWorld(m_world);

    MSG_TRACE_CHANNEL("REFACTOR", "SEND create Render Object message to render system");

    Super::initialise();
}

void Core::onHit()
{
	if (1 < m_ffstrength) //core is not dead yet
	{
		m_ffstrength--;
		m_hit = true;
		//cout << m_ffstrength << endl;
	}
	else
	{
		//Trigger the end sequence
		m_ffstrength--;
		m_active = false;
	}
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void Core::DeserialiseInternal( const tinyxml2::XMLElement* element)
{
    
    const tinyxml2::XMLAttribute* attribute = element->FindAttribute("name");
    if (attribute != nullptr)
    {
        m_name = attribute->Value();
    }

    for (const tinyxml2::XMLElement* childElement = element->FirstChildElement(); childElement; childElement = childElement->NextSiblingElement())
    {
        auto childElementHash = Hashing::hashString(childElement->Value());
        if (childElementHash == Material::m_hash)
        {
            const tinyxml2::XMLAttribute* nameAttribute = childElement->FindAttribute("name"); //This material needs a name to distinguish between normal and glowing versions of the material
            if (nameAttribute)
            {
                if (strICmp(nameAttribute->Value(), "CoreMaterialNormal"))
                {
                    MSG_TRACE_CHANNEL("REFACTOR", "SEND create material message to render system");
                    //m_forcefieldmatnormal.deserialise(m_resource, getResource().getDeviceManager(), getResource().getTextureManager(), getResource().getLightManager(), childElement);
                    //shaderInstance.getMaterial().deserialise(m_resource, getResource().getDeviceManager(), getResource().getTextureManager(), getResource().getLightManager(), childElement);
                    //shaderInstance.getMaterial().addTextureReference(Material::TextureSlotMapping(hashString("cube_player_forcefield"), Material::TextureSlotMapping::Diffuse0));
                }
                else if( strICmp(nameAttribute->Value(), "CoreMaterialNormalGlow") )
                {
                    MSG_TRACE_CHANNEL("REFACTOR", "SEND create material message to render system");
                    //m_forcefieldmatglowing.deserialise(m_resource, getResource().getDeviceManager(), getResource().getTextureManager(), getResource().getLightManager(), childElement);
                }
            }
        }
        else if (childElementHash == "Model"_hash)
        {
            attribute = childElement->FindAttribute("file_name");
            if (attribute != nullptr)
            {
                MSG_TRACE_CHANNEL("REFACTOR", "SEND create material message to render system");
                //Heavily relies on the shader instance existing before we load the model, might be better to put the model construction in initialise instead
                //m_drawableObject = getWriteableResource().getModelManager().LoadModel(m_resource, shaderInstance, attribute->Value());
                //m_drawableObject->setDirty();
            }
        }
    }

    //Get texture strings and load textures
    const SettingsManager* sm = m_resource->m_settingsManager;
    const ISetting<std::string>* textureString = sm->getSetting<std::string>("Core");
    if (textureString)
    {
        MSG_TRACE_CHANNEL("REFACTOR", "SEND create material message to render system");
        //tm.addLoad(getResource().getDeviceManager(), textureString->getData());
        //shaderInstance.getMaterial().addTextureReference(Material::TextureSlotMapping(hashString(getTextureNameFromFileName(textureString->getData())), Material::TextureSlotMapping::Diffuse0));
    }
    textureString = sm->getSetting<std::string>("ForceFieldCore");
    if (textureString)
    {
        MSG_TRACE_CHANNEL("REFACTOR", "SEND create material message to render system");
        //tm.addLoad(getResource().getDeviceManager(), textureString->getData());
        //shaderInstance.getMaterial().addTextureReference(Material::TextureSlotMapping(hashString(getTextureNameFromFileName(textureString->getData())), Material::TextureSlotMapping::Diffuse0));
    }
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void Core::update( float elapsedTime, const Input& input )
{
    if (360 > m_angle)
        m_angle += 10.0f * elapsedTime;
    else
        m_angle = 0.0f;
    if (360 > m_ffangle)
        m_ffangle += 50.0f * elapsedTime;
    else
        m_ffangle = 0.0f;
    m_timepassed += elapsedTime;

    //if (m_active)
    //{
        //ID3D11DeviceContext* deviceContext = DeviceManager::getInstance().getDeviceContext();
        //glPushMatrix();
        //{
        //	glEnable(GL_TEXTURE_2D);
        //	TextureManager::getInstance().activateTexture("..\\Textures\\core.jpg");
        //	glTranslatef(m_position.x(), m_position.y(), m_position.z());
        //	glRotatef(m_angle, 0.0f, 1.0f, 0.0f);
        //	glRotatef(90, 1.0f, 0.0f, 0.0f);
        //	gluSphere(m_core, m_radius, m_slices, m_stacks);
        //}
        //glPopMatrix();
        //if (m_ffstrength > 1)
        //{
        //	glPushMatrix();
        //	{
        //		if (!m_hit)
        //			m_forcefieldmatnormal.activate();
        //		else if (m_timepassed < 0.5f)
        //			m_forcefieldmatnormal.activate();
        //		else if (m_timepassed > 0.5f && m_timepassed < 1.0f)
        //			m_forcefieldmatglowing.activate();
        //		else if (m_timepassed > 1.0f)
        //		{
        //			m_forcefieldmatnormal.activate();
        //			m_timepassed = 0.0f;
        //		}
        //		glDepthMask(false);
        //		glEnable(GL_BLEND);
        //		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //		glEnable(GL_TEXTURE_2D);
        //		glMatrixMode(GL_TEXTURE);
        //		static float texturespeed = 0.0f;
        //		if (texturespeed < 1.0f)
        //			texturespeed += 0.001f;
        //		else
        //			texturespeed = 0.0f;
        //		Matrix44 m;
        //		translate(m, texturespeed, texturespeed, 0.0f);
        //		glLoadMatrixf(m.transposed().m());
        //		glMatrixMode(GL_MODELVIEW);
        //		TextureManager::getInstance().activateTexture(SpaceSim::getInstance().getstd::stringSetting("forcefieldcore"));
        //		glTranslatef(m_position.x(), m_position.y(), m_position.z());
        //		glRotatef(90, 1.0f, 0.0f, 0.0f);
        //		gluSphere(m_coreforcefield, m_radius + 20.0f, m_slices, m_stacks);
        //		glDepthMask(true);
        //		glDisable(GL_TEXTURE_2D);
        //		glMatrixMode(GL_TEXTURE);
        //		glLoadIdentity();
        //		glMatrixMode(GL_MODELVIEW);
        //	}
        //	glPopMatrix();
        //}
        Super::update(elapsedTime, input);
    //}

    //glPointSize(4.0f);
    //glColor4fv(Color::yellow().rgba());
    //glBegin(GL_POintS);
    //(m_position.x(), m_position.y(), m_position.z());
    //glEnd();
    
    UNUSEDPARAM(input);
    UNUSEDPARAM(elapsedTime);
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void Core::handleMessage( const MessageSystem::Message& msg )
{
    UNUSEDPARAM(msg);
    //const ActivationMessage& activateMsg = (const ActivationMessage&)msg;
    //setActive(activateMsg.shouldActivate());
}

void Core::OnDebugImguiInternal()
{
    MaterialConstants materialParameters = m_forcefieldmatnormal.getMaterialCB();
    ImGui::CollapsingHeader("Material");
    ImGui::InputFloat4("Ambient", materialParameters.ambient.GetDataPtr());
    ImGui::InputFloat4("Diffuse", materialParameters.diffuse.GetDataPtr());
    ImGui::InputFloat4("Specular", materialParameters.specular.GetDataPtr());
    ImGui::InputFloat4("Emissive", materialParameters.emissive.GetDataPtr());
    ImGui::InputFloat("Shininess", &materialParameters.shininess);
}
