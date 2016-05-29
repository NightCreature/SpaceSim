#include "switch.h"
#include "matrix44.h"
#include "texturemanager.h"
#include "modelmanager.h"
#include <string>
#include <fstream>

#include "..\SpaceSim\GameResource.h"
#include "..\SpaceSim\GameObjectManager.h"
#include "..\SpaceSim\TypeHelpers.h"
#include "..\SpaceSim\DebugBox.h"

HASH_ELEMENT_IMPLEMENTATION(Switch)

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

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
const ShaderInstance Switch::deserialise( const tinyxml2::XMLElement* element)
{
    ShaderInstance shaderInstance;
    const tinyxml2::XMLAttribute* attribute = element->FindAttribute("name");
    if (attribute != nullptr)
    {
        m_name = attribute->Value();
    }

    unsigned int linkHash = hashString("Link");
    for (element = element->FirstChildElement(); element != 0; element = element->NextSiblingElement())
    {
        unsigned int typeHash = hashString(element->Value());
        if (Material::m_hash == typeHash)
        {
            shaderInstance.getMaterial().deserialise(m_resource, getGameResource().getDeviceManager(), getGameResource().getTextureManager(), getGameResource().getLightManager(), element);
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
        else if( linkHash == typeHash)
        {
            const char* linkedSpecial = element->Attribute("link");
            MSG_TRACE_CHANNEL( "SWITCH", "Hash this name to get a link to the object from the object manager: %s, %d", linkedSpecial, hashString(linkedSpecial) );
            const GameObject* linkedGameObject = getGameResource().getGameObjectManager().getGameObject(linkedSpecial);
            if (linkedGameObject)
            {
                m_special = const_cast<GameObject*>(linkedGameObject); //Need to cast const away here as we need to be able to call a function on the model that is not a const function, should be implemented as a message or event to the actual game object
            }
        }
    }
    return shaderInstance;
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void Switch::update( RenderInstanceTree& renderInstances, float elapsedTime, const Input& input )
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
    Super::update(renderInstances, elapsedTime, input);

    UNUSEDPARAM(input);
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void Switch::handleMessage( const Message& msg )
{
    UNUSEDPARAM(msg);
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void Switch::initialise( const ShaderInstance& shaderInstance )
{
    std::string data = "Models\\switch.dat";//ss.getstd::stringSetting("switchdata");
    if (data.empty())
        return;
    
    m_drawableObject = getWriteableGameResource().getModelManager().LoadModel(m_resource, shaderInstance, data);
    Material mat = Material(0.0f, Color::white(), Color(0.0f, 0.0f, 0.0f, 0.75f), Color(0.0f, 5.0f, 0.0f, 0.75f), Color(0.0f, 1.0f, 0.0f, 0.75f));
    ////mat.setBlendState(true);
    //m_drawableObject->setMaterial(mat);

    m_active = true;

    Super::initialise(shaderInstance);
}
