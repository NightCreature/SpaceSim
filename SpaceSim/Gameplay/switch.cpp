#include "Gameplay/switch.h"
#include "Math/matrix44.h"
#include "Graphics/texturemanager.h"
#include "Graphics/modelmanager.h"
#include <string>
#include <fstream>

#include "Core/Resource/GameResource.h"
#include "Core/Resource/renderResource.h"
#include "Gameplay/GameObjectManager.h"
#include "Core/Types/TypeHelpers.h"
#include "Graphics/DebugBox.h"
#include "imgui.h"
#include <Loader/ModelLoaders/ModelLoader.h>
#include <Core/StringOperations/StringHelperFunctions.h>
#include <Core/Resource/Resourceable.h>
#include <Core/MessageSystem/MessageQueue.h>
#include <Core/MessageSystem/RenderMessages.h>



Switch::Switch(Resource* resource, Vector3 position, GameObject* associatedspecial):
GameObject(resource)
{
	m_active = true;
	m_activemat = true;
	m_prevtime = 0.0f;
	m_position = position;
	m_special = associatedspecial;

	Matrix44 transform, rotated;
	rotate(rotated, Vector3::yAxis(), 180);
	translate(transform ,m_position.x(), m_position.y(), m_position.z());
	m_world  = rotated * transform;
}

void Switch::onHit()
{
	if (m_active)
	{	
		m_active = false;
		m_special->setActive(false); //Should be sending an event here
	}
	else
	{
		m_active = true;
		m_special->setActive(true);
	}
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void Switch::DeserialiseInternal( const tinyxml2::XMLElement* element)
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
            //shaderInstance.getMaterial().deserialise(m_resource, getResource().getDeviceManager(), getResource().getTextureManager(), getResource().getLightManager(), element);
        }
        else if (Vector3::m_hash == typeHash)
        {
            m_position.deserialise(element);
            //translate(m_world, m_position.x(), m_position.y(), m_position.z()); we leave world alone here the whole switch is transformed
            Matrix44 transform, rotated;
            transform.identity();
            rotated.identity();
            rotate(rotated, Vector3::yAxis(), 180);
            translate(transform ,m_position.x(), m_position.y(), m_position.z());
            m_world = rotated * transform;
        }
        else if( "Link"_hash == typeHash)
        {
            const char* linkedSpecial = element->Attribute("link");
            MSG_TRACE_CHANNEL( "SWITCH", "Hash this name to get a link to the object from the object manager: %s, %d", linkedSpecial, Hashing::hashString(linkedSpecial) );
            const GameObject* linkedGameObject = GameResourceHelper(m_resource).getResource().getGameObjectManager().getGameObject(linkedSpecial);
            if (linkedGameObject)
            {
                m_special = const_cast<GameObject*>(linkedGameObject); //Need to cast const away here as we need to be able to call a function on the model that is not a const function, should be implemented as a message or event to the actual game object
            }
        }
        else if (typeHash == Hashing::hashString("Model"))
        {
            attribute = element->FindAttribute("file");
            if (attribute != nullptr)
            {
                //Heavily relies on the shader instance existing before we load the model, might be better to put the model construction in initialise instead
                //m_drawableObject = getWriteableResource().getModelManager().LoadModel(m_resource, shaderInstance, attribute->Value());
                //m_drawableObject->setDirty();
                MSG_TRACE_CHANNEL("REFACTOR", "SEND create material message to render system");

                auto resource = GameResourceHelper(m_resource).getWriteableResource();
                DECLAREANDCREATERESOURCEMESSAGE(createModel, LoadModelResource);
                LoadModelResource param;
                stringCopy(param.m_fileName, attribute->Value());
                createModel.SetData(param);
                createModel.SetGameObjectId(static_cast<size_t>(m_nameHash)); //Not super but should work for now
                resource.m_messageQueues->getUpdateMessageQueue()->addMessage(createModel);
            }
        }
    }
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void Switch::update( float elapsedTime, const Input& input )
{
    m_prevtime += elapsedTime;
    if (0.25f < m_prevtime)
    {
        if (m_activemat && m_active)
        {
            m_activemat = false;
        }
        else
        {
            m_activemat = true;
        }
        m_prevtime = 0.0f;
    }

    //.glActiveTexture(GL_TEXTURE0);
    //glEnable(GL_TEXTURE_2D);
    //if (m_active)
    //{
    //	if (m_activemat)
    //		m_mat1.activate();
    //	else
    //		m_mat2.activate();
    //}
    //else 
    //{
    //	m_material.activate();
    //}
    //TextureManager& tm = TextureManager::getInstance();
    //glEnable(GL_BLEND);
    //glDepthMask(FALSE);

    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //.glActiveTexture(GL_TEXTURE0);
    //glEnable(GL_TEXTURE_2D);
    //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    //tm.activateTexture("..\\Textures\\switch.jpg");

    //.glActiveTexture(GL_TEXTURE1);
    //glEnable(GL_TEXTURE_2D);
    //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    //tm.activateTexture("..\\Textures\\switchalpha.jpg");

    //m_mesh.draw();
    //glDisable(GL_BLEND);
    //glDepthMask(TRUE);
    //.glActiveTexture(GL_TEXTURE0);
    //glDisable(GL_TEXTURE_2D);
    //m_mesh.transform(m_world);
    Super::update(elapsedTime, input);

    MessageSystem::RenderInformation renderInfo;
    MessageSystem::RenderInformation::RenderInfo data;
    data.m_renderObjectid = m_renderHandle;
    data.m_gameobjectid = m_nameHash;
    data.m_world = m_world;
    data.m_name = m_name.c_str();
    data.m_shouldRender = m_active;
    renderInfo.SetData(data);
    m_resource->m_messageQueues->getUpdateMessageQueue()->addMessage(renderInfo);

    UNUSEDPARAM(input);
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void Switch::handleMessage( const MessageSystem::Message& msg )
{
    if (msg.getMessageId() == MESSAGE_ID(CreatedRenderResourceMessage))
    {
        const MessageSystem::CreatedRenderResourceMessage& renderResourceMsg = static_cast<const MessageSystem::CreatedRenderResourceMessage&>(msg);
        renderResourceMsg.GetData();
        m_renderHandle = renderResourceMsg.GetData()->m_renderResourceHandle;
        //Store the render object reference we get back and the things it can do

        //Register the bounding box with the physics
        //GameResourceHelper(m_resource).getWriteableResource().getPhysicsManager().AddColidableBbox(&(m_drawableObject->getBoundingBox()));

        m_initialisationDone = true;
        m_active = true;
    }

}

void Switch::OnDebugImguiInternal()
{
    m_mat1.OnDebugImgui("Deactive Material");
    m_mat2.OnDebugImgui("Active Material");;
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void Switch::initialise()
{
    std::string data = "Models\\switch.dat";//ss.getstd::stringSetting("switchdata");
    if (data.empty())
        return;
    
    //This all needs to be a message to the renderer to create a render object
    MSG_TRACE_CHANNEL("REFACTOR", "CREATE RENDER RESOURCE HERE THROUGH A MESSAGE");
    //m_drawableObject = GameResourceHelper(m_resource).getWriteableResource().getModelManager().LoadModel(m_resource, shaderInstance, data);
    //Material mat = Material(0.0f, Color::white(), Color(0.0f, 0.0f, 0.0f, 0.75f), Color(0.0f, 5.0f, 0.0f, 0.75f), Color(0.0f, 1.0f, 0.0f, 0.75f));
    ////mat.setBlendState(true);
    //m_drawableObject->setMaterial(mat);

    //m_active = true;


    Super::initialise();
}
