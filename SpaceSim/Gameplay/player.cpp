#include "Core/Settings/Settings.h"
#include "Core/Settings/SettingsManager.h"
#include "Core/Types/TypeHelpers.h"
#include "assert.h"
#include "Loader/MapLoader.h"
#include "Graphics/material.h"
#include "Math/matrix44.h"
#include "Graphics/modelmanager.h"
#include "Gameplay/particlesystemmanager.h"
#include "Gameplay/player.h"
#include "Gameplay/scorchmark.h"
#include "Core/Settings/settingsparser.h"

#include "Graphics/CameraManager.h"
#include "Core/Resource/renderResource.h"
#include "Gameplay/LaserManager.h"
#include "Graphics/EffectCache.h"
#include "Core/StringOperations/HashString.h"
#include "Input/InputSystem.h"

const int playerlive = 5;
const int playerffstrength = 10;
const HashString fire("fire");

void Player::initialize(const CameraManager& cameraManager)
{
    m_gameover = false;
    m_count = 0;
    m_lasergentime = 0;
    m_camera = cameraManager.getCamera("player_camera");
    m_hit = false;
    m_camstate = firstperson;
    m_ffangle = 0.0f;
    const ISetting<std::string>* fileNameSettings = m_resource->m_settingsManager->getSetting<std::string>("PlayerSettingsFile");
    if (fileNameSettings != nullptr)
    {
        std::string playerSettingsFileName = fileNameSettings->getData();
        tinyxml2::XMLDocument doc;
        if (doc.LoadFile(playerSettingsFileName.c_str()) != tinyxml2::XML_NO_ERROR)
        {
            MSG_TRACE_CHANNEL("ERROR", "Failed to load %s\nWith error: %d", playerSettingsFileName.c_str(), doc.ErrorID() )
            return;
        }
        
        tinyxml2::XMLElement* root = doc.FirstChildElement();

        //Go to the first child as the first node is the xml node
        tinyxml2::XMLElement* node = root->FirstChildElement();
        if (!strICmp(node->Value(), "Player"))
        {
            MSG_TRACE_CHANNEL("ERROR", "First Element in the file is not a Player element")
            return;
        }

        deserialise(node);
    }
}

Player::~Player()
{
    cleanup();
}

void Player::cleanup()
{
}

void Player::reset()
{
    cleanup();
    m_position = m_startposition;
    //m_camera->positionCamera(m_position, Vector3(m_position.x(), m_position.y(), m_position.z() -(m_position.z() + 1)), Vector3::yAxis());
}

//Bbox Player::calculateBbox()
//{
//    //ModelManager& mm = ModelManager::getInstance();
//    //gxbase::Model* model = mm.getModel(SpaceSim::getInstance().getstd::stringSetting("spaceship"));
//    //gxbase::Model::Vector3f c = model->GetBoxCentre();
//    //gxbase::Model::Vector3f bs = model->GetBoxSize();
//    //Vector3 center(c.x, c.y, c.z);
//    //Vector3 boxsize(bs.x, bs.y, bs.z);
//    //m_bbox.setMax(center + boxsize / 2);
//    //m_bbox.setMin(center - boxsize / 2);
//    //Matrix44 transform;
//    //translate(transform, m_position.x(), m_position.y(), m_position.z());
//    //Vector4 v1(m_bbox.getMin().x(), m_bbox.getMin().y(), m_bbox.getMin().z(), 1.0f);
//    //Vector4 v2(m_bbox.getMax().x(), m_bbox.getMax().y(), m_bbox.getMax().z(), 1.0f);
//    //v1 = v1 * transform;
//    //v2 = v2 * transform;
//    //m_bbox.setMin(Vector3(v1.x(), v1.y(), v1.z()));
//    //m_bbox.setMax(Vector3(v2.x(), v2.y(), v2.z()));
//    return m_bbox;
//}

void Player::loadFromFile(const std::string& filename)
{
    assert(!filename.empty());
    UNUSEDPARAM(filename);

    m_name = "Player";
    m_lives = 3;
    m_ffstrength = 10;
}

void Player::save(const std::string &filename) const
{
    assert(!filename.empty());
    UNUSEDPARAM(filename);
}

void Player::getsHit()
{	
    m_ffstrength--;
    m_hit = true;
    if(isPlayerDead())
        m_gameover = true;
}

void Player::getsKilled()
{
    m_lives--;
    m_position = m_startposition;
    if(isPlayerDead())
        m_gameover = true;
}

bool Player::isPlayerDead()
{
    if (m_lives == 0 || m_gameover)
        return true;
    else
        if (m_ffstrength == 0)
        {
            m_ffstrength = m_defaultffstrength;
            getsKilled();
        }
    return false;
}

void Player::switchviews()
{
    if (m_camstate == firstperson)
        m_camstate = thirdperson;
    else if (m_camstate == thirdperson)
        m_camstate = firstperson;
}

void Player::draw()
{
    if (m_camstate == thirdperson)
    {
        //glPushMatrix();
        //glLoadIdentity();
        //glTranslatef(0.0f, -2.0f, -10.0f);
        //glRotatef(180, 0.0f, 0.0f, 1.0f);
        //glRotatef(90, 1.0f, 0.0f, 0.0f);
        //glDisable(GL_CULL_FACE);
        //ModelManager::getInstance().getModel(SpaceSim::getInstance().getstd::stringSetting("spaceship"))->DrawGL(DSMaterial);
        //drawForceField(m_forcefieldoutside, Vector3(0.0f, -2.0f, -10.0f));
        //glPopMatrix();
        //glEnable(GL_CULL_FACE);
    }
    else
    {
        if (m_hit)
        {
            //glPushMatrix();
            //drawForceField(m_forcefieldinside, m_position);
            //glPopMatrix();
        }	
    }
}

//void Player::drawForceField(GLUquadric* sphere, const Vector3& translate)
//{
    //glDepthMask(false);
    //Material mat(Color(0.1f,1.0f,0.1f,0.75f), Color::black(), Color::black(), 0.0f, Color(0.1f,1.0f,0.1f,0.75f));
    //mat.activate();
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glEnable(GL_TEXTURE_2D);
    //glMatrixMode(GL_TEXTURE);
    //Matrix44 transform;
    //rotate(transform, Vector3(1.0f, 0.5f, 0.0f), m_ffangle);
    //glLoadMatrixf(transform.m());
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    //glTranslatef(translate.x(), translate.y(), translate.z());
    //glRotatef(-90, 1.0f, 0.0f, 0.0f);
    //TextureManager::getInstance().activateTexture(SpaceSim::getInstance().getstd::stringSetting("forcefieldplayer"));
    //gluSphere(sphere, 2.25f, 25, 25);
    //glDisable(GL_BLEND);
    //glDisable(GL_TEXTURE_2D);
    //glMatrixMode(GL_TEXTURE);
    //glLoadIdentity();
    //glMatrixMode(GL_MODELVIEW);
    //glDepthMask(true);
//}

void Player::drawLasersAndScorchmarks()
{
    //Do this in player::update instead
    //m_lasersit = m_lasers.begin();
    //while (m_lasersit != m_lasers.end())
    //{
    //    Laser* l = *m_lasersit;
    //    l->draw(); should be an update call instead
    //    m_lasersit++;
    //}
    //m_scorchit = m_scorchmarks.begin();
    //while (m_scorchit != m_scorchmarks.end())
    //{
    //    ScorchMark* sm = *m_scorchit;
    //    sm->draw(); same as the laser
    //    m_scorchit++;
    //}
}

void Player::drawMiniMap()
{
    //glPointSize(4.0f);
    //glBegin(GL_POintS);
    //(m_position.x(), m_position.y(), m_position.z());
    //glEnd();
}

void Player::fireLaser()
{

    //if (m_count < m_maxlasers)
    //{
    MSG_TRACE_CHANNEL("REFACTOR", "SEND A MESSAGE TO CREATE A RENDER RESOURCE OR REFACTOR THE LASERMANAGER TO DO THIS FOR US");
    //ShaderInstance shaderInstance;
    //shaderInstance.setMaterial(Material(0.0f, Color::black(), Color::black(), Color::green(), Color::green()));
    //shaderInstance.getMaterial().setBlendState(true);
    //shaderInstance.getMaterial().setEffect(getResource().getEffectCache().getEffect("laser_effect.xml"));

    //GameResourceHelper(m_resource).getWriteableResource().getLaserManager().addInstance(m_position, m_direction * 10, ShaderInstance);
    //}
}

void Player::updateLasers(float elapsedtime, MapLoader& m_map)
{
    UNUSEDPARAM(m_map);
    m_lasergentime += elapsedtime;
    m_ffangle += 0.1f;
    m_lasersit = m_lasers.begin();
    //bool switchhit = false;
    while (m_lasersit != m_lasers.end())
    {
        Laser* l = *m_lasersit;
        Vector3 pos = l->getPosition();
        //Vector3 startpos = l->getStartPosition();
        //pos = pos - startpos;
        if (2500.0f > fabs(pos.x()))
            if(2500.0f > fabs(pos.y()))
                if (2500.0f > fabs(pos.z()))
                {
                    //Vector3 planenormal;
                    //if (m_map.checkLaserCollision(l->getBbox(), l->getDirection(), switchhit, planenormal, *this))
                    //{
                    //    if (17.5 > l->getLifeTime() && !switchhit)
                    //    {
                    //        createScorchMark(l->getPosition(), -planenormal);
                    //    }
                    //    ParticleSystemManager* psm = getWriteableResource()->getParticleSystemManager();
                    //    ParticleEmitter* pe = psm->createEmitter(l->getPosition(), planenormal);
                    //    pe->setMaxParticles(5.0f);
                    //    pe->setEmissionRate(5.0f);
                    //    pe->setParticleStartColor(Color::green());
                    //    pe->setParticleKey1Color(Color::green());
                    //    pe->setParticleKey2Color(Color::green());
                    //    pe->setParticelEndColor(Color::green());
                    //    pe->setParticleKey1Pos(0.33f);
                    //    pe->setParticleKey2Pos(0.66f);
                    //    pe->setParticleLifetime(1.0f);
                    //    pe->setLifeTime(1.0f);
                    //    pe->setParticleSize(0.75f);
                    //    pe->setStartVelocity(7.5f);
                    //    pe->setSpreadAngle(90.0f);
                    //    pe->setAlive(true);
                    //    eraseLaser(m_lasersit, l);
                    //    continue;
                    //}
                    ////l->move(elapsedtime);
                    m_lasersit++;
                }
                else
                    eraseLaser(m_lasersit, l);
            else
                eraseLaser(m_lasersit, l);
        else
            eraseLaser(m_lasersit, l);
    }
    m_scorchit = m_scorchmarks.begin();
    while (m_scorchit != m_scorchmarks.end())
    {
        ScorchMark* sm = *m_scorchit;
        if (sm->getLifetime() < 0.0f)
        {
            delete sm;
            m_scorchit = m_scorchmarks.erase(m_scorchit);
            continue;
        }
        //sm->move(elapsedtime); Should be done in update of scorchmark
        m_scorchit++;
    }
}

void Player::eraseLaser(std::list<Laser*>::iterator& it, Laser* l)
{
    delete l;
    it = m_lasers.erase(it);
    m_count--;
}

void Player::createScorchMark(const Vector3 &pos, const Vector3 &normal)
{
    m_scorchmarks.push_back(new ScorchMark(m_resource, pos, normal, 5.0f));
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void Player::update( RenderInstanceTree& renderInstances, float elapsedTime, const Input& input )
{
    PROFILE_EVENT("Player::update", Green);
    //Should do move update and render list creation here
    UNUSEDPARAM(renderInstances);
    UNUSEDPARAM(elapsedTime);
    UNUSEDPARAM(input);

    MSG_TRACE_CHANNEL("REFACTOR", "NEED A CAMERA ON THE UPDATE THREAD SOMEHOW FOR THIS");
    //m_position = m_camera->getEye();
    //m_direction = m_camera->getLookAt();

	InputActions::ActionType inputAction;
	InputSystem::getInputActionFromName(fire.getHash(), inputAction);
	if (input.getInput(0) && input.getInput(0)->getActionValue(inputAction) > 0.0f)
	{
		if (m_lasergentime > 0.25)
		{
			m_lasergentime = 0;
			fireLaser();
		}
	}

    m_lasergentime += elapsedTime;
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
const ShaderInstance Player::deserialise( const tinyxml2::XMLElement* node )
{
    const tinyxml2::XMLAttribute* attribute = node->FindAttribute("name");
    if (attribute)
    {
        m_name = attribute->Value();
    }
    attribute = node->FindAttribute("game_object_name");
    if (attribute)
    {
        m_gameObjectName = attribute->Value();
    }
    attribute = node->FindAttribute("number_of_lives");
    if (attribute)
    {
        m_lives = attribute->IntValue();
    }
    attribute = node->FindAttribute("force_field_strength");
    if (attribute)
    {
        m_ffstrength = attribute->IntValue();
    }

    for (const tinyxml2::XMLElement* childElement = node->FirstChildElement(); childElement != nullptr; childElement = childElement->NextSiblingElement())
    {
        unsigned int childElementHash = hashString(childElement->Value());
        if ( Vector3::m_hash == childElementHash )
        {
            const tinyxml2::XMLAttribute* childAttribute = childElement->FindAttribute("name");
            if (strICmp(childAttribute->Value(), "start_position"))
            {
                m_position.deserialise(childElement);
                m_startposition.deserialise(childElement);
            }
        }
    }

    return ShaderInstance();
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
bool Player::collision( const Bbox& bbox, const Vector3& dir )
{
    UNUSEDPARAM(bbox);
    UNUSEDPARAM(dir);
    return false;
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
float Player::collision( const Vector3& position, const Vector3& dir, Vector3& normal )
{
    UNUSEDPARAM(position);
    UNUSEDPARAM(dir);
    UNUSEDPARAM(normal);
    return -1.0f;
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void Player::handleMessage( const MessageSystem::Message& msg )
{
    UNUSEDPARAM(msg);
}
