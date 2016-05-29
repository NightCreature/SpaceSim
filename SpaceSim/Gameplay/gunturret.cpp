#include "gunturret.h"
#include "..\SpaceSim\SettingsManager.h"
#include "particleemitter.h"
#include "particlesystemmanager.h"
#include "matrix44.h"
#include "box.h"
#include "modelmanager.h"
#include "mesh.h"
#include "../SpaceSim/LaserManager.h"
#include <iostream>
#include <assert.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <vector>
#include "../SpaceSim/GameResource.h"
#include "../SpaceSim/EffectCache.h"
#include "../SpaceSim/StringHelperFunctions.h"
#include "../SpaceSim/DebugBox.h"

HASH_ELEMENT_IMPLEMENTATION(GunTurret)


GunTurret::GunTurret(Resource* resource, const Vector3& position, const Vector3& direction):
GameObject(resource)
{
    m_position = position;
    m_direction = direction;
    m_lasergentime = 0;
    m_active = true;
    m_lasers.clear();
    m_scorchmarks.clear();
    m_world.identity();
}

GunTurret::~GunTurret()
{
    cleanup();
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void GunTurret::initialise(const ShaderInstance& shaderInstance)
{
    UNUSEDPARAM(shaderInstance);

    const SettingsManager& sm = getGameResource().getSettingsManager();
    const ISetting<std::string>* modelString = sm.getSetting<std::string>("Guns");
    if (modelString)
    {
        m_drawableObject = getWriteableGameResource().getModelManager().LoadModel(m_resource, shaderInstance, modelString->getData());
    }

    Super::initialise(shaderInstance);
}

void GunTurret::cleanup()
{
}

void GunTurret::onHit()
{
    if (m_active)
    {
        ParticleSystemManager& psm = getWriteableGameResource().getParticleSystemManager();
        ParticleEmitter* pe = psm.createEmitter(m_center, Vector3::yAxis());
        pe->setMaxParticles(250.0f);
        pe->setEmissionRate(75.0f);
        pe->setParticleStartColor(Color(1.0f, 1.0f, 1.0f, 1.0f));//Color(0.75f, 0.5f, 0.01f, 0.75f));
        pe->setParticleKey1Color(Color(1.0f, 1.0f, 0.0f, 0.8f));
        pe->setParticleKey1Pos(0.001f);
        pe->setParticleKey2Color(Color(1.0f, 0.5f, 0.0f, 0.6f));
        pe->setParticleKey2Pos(0.25f);
        pe->setParticelEndColor(Color(0.0f, 0.0f, 0.0f, 0.0f));//Color(0.99f, 0.2f, 0.1f, 0.0f));
        pe->setParticleLifetime(1.0f);
        pe->setLifeTime(5.0f);
        pe->setParticleSize(7.5f);
        pe->setStartVelocity(10.0f);
        pe->setSpreadAngle(180.0f);
        pe->setAlive(true);
        m_active = false;
    }
}

void GunTurret::transformBoundingBox()
{
    ////Create the 8 points of the box and enclose them in the bounding box
    //Vector3 min = m_bbox.getMin();
    //Vector3 max = m_bbox.getMax();
    //Vector4 bbox[8];
    //bbox[0] = Vector4(min.x(), min.y(), min.z(), 0.0f);
    //bbox[1] = Vector4(max.x(), min.y(), min.z(), 0.0f);
    //bbox[2] = Vector4(max.x(), max.y(), min.z(), 0.0f);
    //bbox[3] = Vector4(min.x(), max.y(), min.z(), 0.0f);
    //bbox[4] = Vector4(max.x(), min.y(), max.z(), 0.0f);
    //bbox[5] = Vector4(min.x(), min.y(), max.z(), 0.0f);
    //bbox[6] = Vector4(min.x(), max.y(), max.z(), 0.0f);
    //bbox[7] = Vector4(max.x(), max.y(), max.z(), 0.0f);
    //Matrix44 transform, rotated;
    //transform.identity(); rotated.identity();
    //if (-Vector3::xAxis() == m_direction)
    //{
    //    rotate(transform, Vector3::yAxis(), -180);
    //}
    //else if (Vector3::yAxis() == m_direction)
    //{
    //    rotate(transform, Vector3::zAxis(), -90);
    //}
    //else if (-Vector3::yAxis() == m_direction)
    //{
    //    rotate(transform, Vector3::zAxis(), 90);
    //}
    //else if (Vector3::zAxis() == m_direction)
    //{
    //    rotate(transform, Vector3::yAxis(), 90);
    //}
    //else if (-Vector3::zAxis() == m_direction)
    //{
    //    rotate(transform, Vector3::yAxis(), -90);
    //}
    //rotate(rotated, Vector3::xAxis(), 90);
    //transform = rotated * transform;
    //m_bbox.empty();
    //for (unsigned int i = 0; i < 8; i++)
    //{
    //    bbox[i] = bbox[i]*transform;
    //    Vector3 v(bbox[i].x() + m_position.x(), bbox[i].y() + m_position.y(), bbox[i].z() + m_position.z());
    //    m_bbox.enclose(v);
    //}
}


void GunTurret::fireLaser()
{
    ShaderInstance shaderInstance;
    shaderInstance.setMaterial(Material(0.0f, Color::black(), Color::black(), Color::red(), Color::red()));
    shaderInstance.getMaterial().setBlendState(true);
    shaderInstance.getMaterial().setEffect(getGameResource().getEffectCache().getEffect("laser_effect.xml"));

    getWriteableGameResource().getLaserManager().addInstance(m_position + Vector3(0.0f, 3.0f, 0.0f), m_direction, shaderInstance);
}

void GunTurret::updateLasers(float elapsedtime/*, MapLoader& m_map, Player& p*/)
{
    m_lasergentime += elapsedtime;
    m_lasersit = m_lasers.begin();
    //bool switchhit = false;
    while (m_lasersit != m_lasers.end())
    {
        //cout << "Number of lasers: " << m_lasers.size();
        Laser* l = *m_lasersit;
        Vector3 pos = l->getPosition();
        //Vector3 startpos = l->getStartPosition();
        //pos = pos - startpos;
        if (2500.0f > fabs(pos.x()))
            if(2500.0f > fabs(pos.y()))
                if (2500.0f > fabs(pos.z()))
                {
                    Vector3 planenormal;
                    //if (m_map.checkLaserCollision(l->getBbox(), l->getDirection(), switchhit, planenormal, p, false))
                    //{
                    //    if (17.5 > l->getLifeTime() && !switchhit)
                    //    {
                    //        //Create scorch mark at the spot of the laser
                    //        createScorchMark(l->getPosition(), -planenormal);
                    //    }
                    //    //Setup particle system when laser hits something
                    //    ParticleSystemManager* psm = getWriteableGameResource()->getParticleSystemManager();
                    //    ParticleEmitter* pe = psm->createEmitter(l->getPosition(), planenormal);
                    //    pe->setMaxParticles(5.0f);
                    //    pe->setEmissionRate(5.0f);
                    //    pe->setParticleStartColor(Color::red());
                    //    pe->setParticleKey1Color(Color::red());
                    //    pe->setParticleKey2Color(Color::red());
                    //    pe->setParticelEndColor(Color::red());
                    //    pe->setParticleKey1Pos(0.33f);
                    //    pe->setParticleKey2Pos(0.66f);
                    //    pe->setParticleLifetime(1.0f);
                    //    pe->setLifeTime(1.0f);
                    //    pe->setParticleSize(0.75f);
                    //    pe->setStartVelocity(7.5f);
                    //    pe->setSpreadAngle(90.0f);
                    //    pe->setAlive(true);
                    //    //MSG_TRACE_CHANNEL("", sizeof(ParticleEmitter) << endl;
                    //    eraseLaser(m_lasersit, l);
                    //    continue;
                    //}
                    //l->move(elapsedtime);
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
        //cout << "Number of lasers: " << m_lasers.size();
        ScorchMark* sm = *m_scorchit;
        if (sm->getLifetime() < 0.0f)
        {
            delete sm;
            m_scorchit = m_scorchmarks.erase(m_scorchit);
            continue;
        }
        //sm->move(elapsedtime); update of the scorch mark takes care of this
        m_scorchit++;
    }
}

//bool GunTurret::checkLaserCollisionPlayer(Laser *l, Player p)
//{
//}


void GunTurret::eraseLaser(LaserList::iterator& it, Laser* l)
{
    delete l;
    it = m_lasers.erase(it);
}

void GunTurret::createScorchMark(const Vector3 &pos, const Vector3 &normal)
{
    //cout <<"Create scorch mark at: " << pos << "\nThis plane has the normal: " << normal << endl;
    //cout << "Number of lasers: " << m_lasers.size();
    MSG_TRACE_CHANNEL("ERROR", "Size of ScorchMark: %d", sizeof(ScorchMark) )
    m_scorchmarks.push_back(new ScorchMark(m_resource, pos, normal, 5.0f));
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
const ShaderInstance GunTurret::deserialise( const tinyxml2::XMLElement* element)
{
    ShaderInstance shaderInstance;
    const tinyxml2::XMLAttribute* attribute = element->FindAttribute("name");
    if (attribute != nullptr)
    {
        m_name = attribute->Value();
    }

    Matrix44 scaleTransform, translation, rotation;
    for (const tinyxml2::XMLElement* childElement = element->FirstChildElement(); childElement; childElement = childElement->NextSiblingElement())
    {
        unsigned int childElementHash = hashString(childElement->Value());
        if (childElementHash == Vector3::m_hash)
        {
            const tinyxml2::XMLAttribute* nameAttribute = childElement->FindAttribute("name"); //This material needs a name to distinguish between normal and glowing versions of the material
            if (nameAttribute)
            {
                if (strICmp(nameAttribute->Value(), "position"))
                {
                    m_position.deserialise(childElement);
                    
                    scale(scaleTransform, 5.0f, 5.0f, 5.0f);
                    translate(translation, m_position);  
                }
                else if( strICmp(nameAttribute->Value(), "direction") )
                {
                    m_direction.deserialise(childElement);
                    m_direction.normalize();
                    rotation = rotationFromDirection(m_direction);
                }
            }
        }
        if (childElementHash == hashString("Guns"))
        {
            const char* modelString = childElement->GetText();
            UNUSEDPARAM(modelString);
            MSG_TRACE_CHANNEL("", "Model string for the GunTurret is: %s", modelString );
        }
        if (childElementHash == Material::m_hash)
        {
            shaderInstance.getMaterial().deserialise(m_resource, getGameResource().getDeviceManager(), getGameResource().getTextureManager(), getGameResource().getLightManager(), childElement );
        }
    }

    m_world = rotation * scaleTransform * translation;

    return shaderInstance;
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void GunTurret::update( RenderInstanceTree& renderInstances, float elapsedTime, const Input& input )
{
    //From draw
    if (m_active)
    {
        //glEnable(GL_LIGHTING);
        //glFrontFace(GL_CCW);
        //glEnable(GL_CULL_FACE);
        //glPushMatrix();
        //glTranslatef(m_position.x(), m_position.y(), m_position.z());
        ////add own rotaion here
        //if (-Vector3::xAxis() == m_direction)
        //{
        //	glRotatef(180, 0.0f, 1.0f, 0.0f);
        //}
        //else if (Vector3::yAxis() == m_direction)
        //{
        //	glRotatef(90, 0.0f, 0.0f, 1.0f);
        //}
        //else if (-Vector3::yAxis() == m_direction)
        //{
        //	glRotatef(-90, 0.0f, 0.0f, 1.0f);
        //}
        //else if (Vector3::zAxis() == m_direction)
        //{
        //	glRotatef(-90, 0.0f, 1.0f, 0.0f);
        //}
        //else if (-Vector3::zAxis() == m_direction)
        //{
        //	glRotatef(90, 0.0f, 1.0f, 0.0f);
        //}

        //glRotatef(-90, 1.0f, 0.0f, 0.0f);
        //ModelManager& mm = ModelManager::getInstance();
        //Model* model = mm.getModel("..\\Models\\gun.3DS");//memory leaks
        //if (0 == model)
        //{
        //	cerr << "Model ..\\Models\\gun.3DS wasn't loaded" << endl;
        //	return;
        //}
        //model->DrawGL(DSMaterial);
        //glPopMatrix();
        //model = 0;

        //m_lasersit = m_lasers.begin();
        //while (m_lasersit != m_lasers.end())
        //{
        //	//cout << "Number of lasers: " << m_lasers.size();
        //	Laser* l = *m_lasersit;
        //	l->draw();
        //	m_lasersit++;
        //}
        //m_scorchit = m_scorchmarks.begin();
        //Material mat(Color::white(), Color::white(), Color::white(), 0);
        //mat.activate();
        //while (m_scorchit != m_scorchmarks.end())
        //{
        //	//cout << "Number of lasers: " << m_lasers.size();
        //	ScorchMark* sm = *m_scorchit;
        //	sm->draw();
        //	m_scorchit++;
        //}
        //glCullFace(GL_CW);
        //glDisable(GL_LIGHTING);
        //
        //updateLasers( elapsedTime );

        if (m_lasergentime > 1.5f)
        {
            m_lasergentime = 0;
            //Direction of the laser is wrong and the time seems pretty broken as well
            //fireLaser();
        }
        m_lasergentime += elapsedTime;

    }
    //From drawminimap
    //glPointSize(4.0f);
    //glColor4fv(Color::red().rgba());
    //glBegin(GL_POintS);
    //(m_position.x(), m_position.y(), m_position.z());
    //glEnd();

    Super::update(renderInstances, elapsedTime, input);

    //for (std::vector<Laser*>::iterator lit = m_lasers.end(); lit != m_lasers.end(); ++lit)
    //{
    //    (*lit)->update(elapsedTime);
    //}
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void GunTurret::handleMessage( const Message& msg )
{
    const ActivationMessage& activateMsg = (const ActivationMessage&)msg;
    setActive(activateMsg.shouldActivate());
}