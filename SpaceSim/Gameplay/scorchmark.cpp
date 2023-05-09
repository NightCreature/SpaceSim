#include "Gameplay/scorchmark.h"
#include "Core/Settings/SettingsManager.h"
#include "Graphics/texturemanager.h"

#include "Core/MessageSystem/MessageQueue.h"
#include "Core/MessageSystem/GameMessages.h"
#include "Core/MessageSystem/RenderMessages.h"

#include "Core/Resource/GameResource.h"

ScorchMark::ScorchMark(Resource* resource, const Vector3& position, const Vector3& normal, float lifetime):
GameObject(resource)
{
	m_position = position;
	m_normal = normal;
	m_lifetime = lifetime;
	Color green = Color(0, 1, 0, 1);
    
    Matrix44 transform;
    if (Vector3::xAxis() == m_normal)
    {
        rotate(m_world, Vector3::yAxis(), 90);
        translate(transform, m_position.x(), m_position.y(), m_position.z());
    }
    else if (Vector3::yAxis() == m_normal)
    {
        rotate(m_world, Vector3::xAxis(), 90);
        translate(transform, m_position.x(), m_position.y(), m_position.z());
    }
    m_world = m_world * transform;
}
///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void ScorchMark::initialise()
{
    Square::SquareCreationParams params;
    params.m_resource = m_resource;
    params.m_lowerleft = Vector2(-0.5f, -0.5f);
    params.m_lowerleft = Vector2(0.5f, 0.5f);

    DECLAREANDCREATERESOURCEMESSAGE(createPlaneModel, Square::SquareCreationParams);
    createPlaneModel.SetGameObjectId(static_cast<size_t>(m_nameHash)); //Not super but should work for now
    createPlaneModel.SetData(params);
    GameResourceHelper(m_resource).getWriteableResource().m_messageQueues->getUpdateMessageQueue()->addMessage(createPlaneModel); //Init isnt done here because we are waiting for a response from the render thread



    Super::initialise();
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void ScorchMark::deserialise( const tinyxml2::XMLElement* node )
{
    //TextureManager& tm = TextureManager::getInstance();
    //SettingsManager& sm = SettingsManager::getInstance();
    ////Get texture strings and load textures
    //const ISetting<std::string>* textureString = sm.getSetting<std::string>("ScorchmakerPlayer");
    //if (textureString)
    //{
    //    tm.addLoad(textureString->getData());
    //}
    //textureString = sm.getSetting<std::string>("ScorchmarkGun");
    //if (textureString)
    //{
    //    tm.addLoad(textureString->getData());
    //}
    //textureString = sm.getSetting<std::string>("ScorchmarkAlpha");
    //if (textureString)
    //{
    //    tm.addLoad(textureString->getData());
    //}

    (void*)node; //Should implement this function
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void ScorchMark::update( float elapsedTime, const Input& input )
{
    m_lifetime -= elapsedTime;

    //glDepthMask(FALSE);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //TextureManager& tm = TextureManager::getInstance();
    //.glActiveTexture(GL_TEXTURE0);
    //glEnable(GL_TEXTURE_2D);
    //if (m_playerscorch)
    //	tm.activateTexture(SpaceSim::getInstance().getstd::stringSetting("scorchmakerplayer"));
    //else
    //	tm.activateTexture(SpaceSim::getInstance().getstd::stringSetting("scorchmarkgun"));
    //.glActiveTexture(GL_TEXTURE1);
    //glEnable(GL_TEXTURE_2D);
    //tm.activateTexture(SpaceSim::getInstance().getstd::stringSetting("scorchmarkalpha"));
    //glPushMatrix();
    //glTranslatef(m_position.x(), m_position.y(), m_position.z());
    //if (Vector3::xAxis() == m_normal)
    //{
    //	glRotatef(-90, 0.0f, 1.0f, 0.0f);
    //}
    //else if (Vector3::yAxis() == m_normal)
    //{
    //	glRotatef(90, 1.0f, 0.0f, 0.0f);
    //}
    //else if (-Vector3::xAxis() == m_normal)
    //{
    //	glRotatef(90, 0.0f, 1.0f, 0.0f);
    //}
    //else if (-Vector3::yAxis() == m_normal)
    //{
    //	glRotatef(-90, 1.0f, 0.0f, 0.0f);
    //}
    //else if (Vector3::zAxis() == m_normal)
    //	glRotatef(180, 0.0f, 1.0f, 0.0f);
    //m_scorchmark.setColor(m_material.getDiffuse());
    //m_scorchmark.draw();
    //glPopMatrix();
    //.glActiveTexture(GL_TEXTURE1);
    //glDisable(GL_TEXTURE_2D);
    //.glActiveTexture(GL_TEXTURE0);
    //glDisable(GL_TEXTURE_2D);
    //glDisable(GL_TEXTURE_2D);
    //glDisable(GL_BLEND);
    //glDepthMask(TRUE);
    UNUSEDPARAM(input);
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void ScorchMark::handleMessage( const MessageSystem::Message& msg )
{
    UNUSEDPARAM(msg);
    if (msg.getMessageId() == MESSAGE_ID(CreatedRenderResourceMessage))
    {
        const MessageSystem::CreatedRenderResourceMessage& renderResourceMsg = static_cast<const MessageSystem::CreatedRenderResourceMessage&>(msg);
        renderResourceMsg.GetData();
        m_renderHandle = renderResourceMsg.GetData()->m_renderResourceHandle;
        //Store the render object reference we get back and the things it can do
        m_initialisationDone = true;
    }
}
