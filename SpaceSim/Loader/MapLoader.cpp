#include "Loader/MapLoader.h"
#include "Gameplay/box.h"
#include "Gameplay/door.h"
#include "Gameplay/forcefield.h"
#include "Gameplay/rotatingblades.h"
#include "Gameplay/switch.h"
#include "Gameplay/core.h"
#include "Gameplay/gunturret.h"
#include "Graphics/material.h"
#include "Graphics/color.h"
#include "Gameplay/particlesystemmanager.h"
#include <limits>
#include <Windows.h>
#include "assert.h"
#include "Graphics/modelmanager.h"
#include "Graphics/DeviceManager.h"
#include "Core/StringOperations/StringHelperFunctions.h"

#include "Core/tinyxml2.h"
#include "Gameplay/GameObjectManager.h"
#include "Graphics/LightManager.h"
#include "Gameplay/InfinitySphere.h"


const unsigned int MapLoader::m_wallHash = hashString("Wall");
const unsigned int MapLoader::m_doorHash = hashString("Door");
const unsigned int MapLoader::m_rotatingBladesHash = hashString("RotatingBlades");
const unsigned int MapLoader::m_switchHash = hashString("Switch");
const unsigned int MapLoader::m_coreHash = hashString("Core");
const unsigned int MapLoader::m_forceFieldHash = hashString("ForceField");

MapLoader::~MapLoader()
{
    cleanup();
}

void MapLoader::cleanup()
{
    m_planes.clear();
}

void MapLoader::reset()
{
    cleanup();
}

//Just transform this to reading of an xml file more structure and less debugging/easier to debug in the end
bool MapLoader::loadMap(Resource* resource, const std::string& filename)
{
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_NO_ERROR)
    {
        MSG_TRACE_CHANNEL("MAP", "Failed to load %s \nWith error: %d", filename.c_str(), doc.ErrorID() )
        return false;
    }

    const tinyxml2::XMLElement* element;
    element = doc.FirstChildElement();

    element = element->FirstChildElement();
    GameResourceHelper gameResourceHelper(resource);
    GameObjectManager& gameObjectManager = gameResourceHelper.getWriteableResource().getGameObjectManager();

    MapContainer map;

    while (element != 0)
    {
        unsigned int elementHash = hashString( element->Value() );
        if ( MapLoader::m_wallHash == elementHash )
        {
            readWallElement(resource, element);
        }
        else if (ForceField::m_hash == elementHash)
        {
            ForceField* forceField = new ForceField(resource);
            ShaderInstance instance = forceField->deserialise(element);
            forceField->initialise(instance, false);
            map.insertSpecial(forceField);
            gameObjectManager.addGameObject(forceField);
        }
        else if (Door::m_hash == elementHash)
        {
            Door* door = new Door(resource);
            ShaderInstance instance = door->deserialise(element);
            door->initialise(instance, false);
            map.insertSpecial(door);
            gameObjectManager.addGameObject(door);
        }
        else if (RotatingBlades::m_hash == elementHash)
        {
            RotatingBlades* rotatingBlades = new RotatingBlades(resource);
            ShaderInstance instance = rotatingBlades->deserialise(element);
            rotatingBlades->initialise(instance, false);
            map.insertSpecial(rotatingBlades);
            gameObjectManager.addGameObject(rotatingBlades);
        }
        else if (Core::m_hash == elementHash)
        {
            Core* core = new Core(resource);
            ShaderInstance instance = core->deserialise(element);
            core->initialise(instance);
            map.insertSpecial(core);
            gameObjectManager.addGameObject(core);
        }
        else if (Switch::m_hash == elementHash)
        {
            MSG_TRACE_CHANNEL("MAP", "Map loading switch");
            Switch* switchObject = new Switch(resource);
            const ShaderInstance shaderInstance = switchObject->deserialise(element);
            switchObject->initialise(shaderInstance);
            map.insertSpecial(switchObject);
            gameObjectManager.addGameObject(switchObject);
        }
        else if (GunTurret::m_hash == elementHash)
        {
            GunTurret* gun = new GunTurret(resource);
            const ShaderInstance shaderInstance = gun->deserialise(element);
            gun->initialise(shaderInstance);
            map.insertSpecial(gun);
            gameObjectManager.addGameObject(gun);
        }
        else if (Light::m_hash == elementHash)
        {
            const tinyxml2::XMLAttribute* attribute = element->FindAttribute("name");
            std::string lightName = "Light";
            if (attribute != nullptr)
            {
                lightName = attribute->Value();
            }
            Light light;
            light.deserialise(element);
            MSG_TRACE_CHANNEL("REFACTOR", "SEND message to add a light to the render side");
            //lightManager.addLight(lightName, light); //This needs to change to add a message to the message system to add a light in the render scene
        }
        else if (InfinitySphere::m_hash == elementHash)
        {
            InfinitySphere* infSphere = new InfinitySphere(resource);
            infSphere->initialise(infSphere->deserialise(element));
            gameObjectManager.addGameObject(infSphere);
        }
        element = element->NextSiblingElement();
    }

    MSG_TRACE_CHANNEL("MAP", "Level has been read in!");
    return true;
}

bool MapContainer::checkCollision(const Bbox& bbox, const Vector3& dir, Player& p, bool& special, Vector3& planenormal)
{
    UNUSEDPARAM(bbox);
    UNUSEDPARAM(dir);
    UNUSEDPARAM(p);
    UNUSEDPARAM(special);
    UNUSEDPARAM(planenormal);
    //PlanesMap::iterator pmit = m_planes.begin();
    //bool collision = false;
    //while (pmit != m_planes.end())
    //{
    //    for (unsigned int i = 0; i < pmit->second.size() && !collision; i++)
    //    {
    //        collision = pmit->second[i]->collision(bbox, dir);
    //        getCollisionNormal(pmit, planenormal);
    //    }
    //    pmit++;
    //}
    //ShapeMap::iterator smit = m_specials.begin();
    //bool specialcollision = false;
    //while (smit != m_specials.end())
    //{
    //    for (unsigned int i = 0; i < smit->second.size() && !specialcollision; i++)
    //    {
    //        specialcollision = smit->second[i]->collision(bbox, dir);
    //        if (specialcollision && smit->second[i]->getActive() && smit->first != SWITCH)
    //        {
    //            p.getsKilled();
    //            special = true;
    //            collision = true;
    //        }
    //    }
    //    smit++;
    //}
    return false;
}

bool MapContainer::checkLaserCollision(const Bbox& v, const Vector3& dir, bool& special, Vector3& planenormal, Player& p, bool lasertype)
{
    UNUSEDPARAM(lasertype);
    UNUSEDPARAM(dir);
    UNUSEDPARAM(p);
    UNUSEDPARAM(v);
    UNUSEDPARAM(special);
    UNUSEDPARAM(planenormal);
    //PlanesMap::iterator pmit = m_planes.begin();
    //bool collision = false;

    //if (!lasertype)
    //{
    //    //Check if a gun turrets laser collides with the player
    //    //MSG_TRACE_CHANNEL("", intersectMovingBboxes(p->getBbox(), v, dir + p->getDirection()) << std::endl;
    //    //if ( 0.0f <= intersectMovingBboxes(p->getBbox(), v, p->getDirection() - dir))
    //    //{
    //    //	//Collision occured so hit the player
    //    //	p->getsHit();
    //    //}
    //    if (v.contains(p.getBoundingBox()))
    //        p.getsHit();
    //    //if (p->getPosition.x() > v.getMin().x() || p->getPosition.x() < v.getMax().x())
    //}

    //while (pmit != m_planes.end())
    //{
    //    for (unsigned int i = 0; i < pmit->second.size() && !collision; i++)
    //    {
    //        collision = pmit->second[i]->collision(v, dir);
    //        if (collision)
    //            getCollisionNormal(pmit, planenormal);
    //    }
    //    pmit++;
    //}
    //ShapeMap::iterator smit = m_specials.begin();
    //bool specialcollision = false;
    //while (smit != m_specials.end())
    //{
    //    for (unsigned int i = 0; i < smit->second.size() && !specialcollision; i++)
    //    {
    //        if (lasertype)
    //        {
    //            specialcollision = smit->second[i]->collision(v, dir);
    //            if (specialcollision && smit->second[i]->getActive() && smit->first != SWITCH)
    //            {
    //                special = true;
    //                collision = true;
    //            }
    //            if (specialcollision && smit->first == SWITCH)
    //            {
    //                MSG_TRACE_CHANNEL("MAP", "switch collision" )
    //                special = true;
    //                collision = true;
    //                Switch* s = (Switch*)smit->second[i];
    //                s->onHit();
    //            }
    //            if (specialcollision && smit->first == CORE)
    //            {
    //                MSG_TRACE_CHANNEL("MAP", "core collision" )
    //                special = true;
    //                collision = true;
    //                Core* c = (Core*)smit->second[i];
    //                c->onHit();
    //                if (c->getFFStrength() <= 0 && !m_exitshowing)
    //                {
    //                    //vector<Plane*>::iterator pit = m_planes[FRONT_WALL].end();
    //                    //pit--;
    //  //                  Plane* plane = *pit;
    //                    //float width = plane->getPlane().getWidth() / 2;
    //                    //float height = plane->getPlane().getHeight() / 2;
    //                    //Vector3 pos = plane->getPlane().getWorld()[3];
    //                    //ParticleSystemManager& psm = ParticleSystemManager::getInstance();
    //                    //pit = m_planes[FRONT_WALL].erase(pit);
    //                    //m_exitshowing = true;
    //                    //ParticleEmitter* pe3 =psm.createEmitter(c->getPosition(), Vector3::yAxis());
    //                    //pe3->setMaxParticles(5000.0f);
    //                    //pe3->setEmissionRate(500.0f);
    //                    //pe3->setParticleStartColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
    //                    //pe3->setParticleKey1Color(Color(1.0f, 1.0f, 0.0f, 0.8f));
    //                    //pe3->setParticleKey1Pos(0.001f);
    //                    //pe3->setParticleKey2Color(Color(1.0f, 0.5f, 0.0f, 0.6f));
    //                    //pe3->setParticleKey2Pos(0.25f);
    //                    //pe3->setParticelEndColor(Color(0.0f, 0.0f, 0.0f, 0.0f));
    //                    //pe3->setParticleLifetime(5.0f);
    //                    //pe3->setLifeTime(10.0f);
    //                    //pe3->setParticleSize(7.5f);
    //                    //pe3->setStartVelocity(15.0f);
    //                    //pe3->setSpreadAngle(180.0f);
    //                    //pe3->setAlive(true);
    //                }
    //            }
    //            if (specialcollision && smit->first == GUNTURRETS)
    //            {
    //                MSG_TRACE_CHANNEL("MAP", "Gun collision" )
    //                special = true;
    //                collision = true;
    //                GunTurret* gt = (GunTurret*)smit->second[i];
    //                gt->setActive(false);                    
    //                gt->onHit();
    //            }
    //        }
    //    }
    //    smit++;
    //}
    return false;
}

void MapLoader::getCollisionNormal(PlanesMap::const_iterator& it, Vector3& normal)
{
    switch (it->first)
    {
    case CEILING:
    case OUTSIDEBOTTOM:
        normal.setY(-1.0f);
        break;
    case FLOOR:
    case OUTSIDETOP:
        normal.setY(1.0f);
        break;
    case LEFT_WALL:
    case OUTSIDERIGHT:
        normal.setX(1.0f);
        break;
    case RIGHT_WALL:
    case OUTSIDELEFT:
        normal.setX(-1.0f);
        break;
    case FRONT_WALL:
    case OUTSIDEBACK:
        normal.setZ(-1.0f);
        break;
    case BACK_WALL:
    case OUTSIDEFRONT:
        normal.setZ(1.0f);
        break;
    }
}

//-------------------------------------------------------------------------
// @brief This should all move in to the plane class
//-------------------------------------------------------------------------
void MapLoader::readWallElement( Resource* resource, const tinyxml2::XMLElement* element )
{
    const tinyxml2::XMLElement* childElement = element->FirstChildElement();
    Material mat;
    Vector3 corners[4];
    unsigned int numberCornersRead = 0;
    while (childElement != 0)
    {
        unsigned int childElementHash = hashString(childElement->Value());
        if (Vector3::m_hash == childElementHash)
        {
            corners[numberCornersRead].deserialise(childElement);
            ++numberCornersRead;
        }

        childElement = childElement->NextSiblingElement();
    }

    assert(numberCornersRead == 4);

    const tinyxml2::XMLAttribute* typeAttribute = element->FirstAttribute();
    int type =  0;
    if (typeAttribute)
    {
        type = typeAttribute->IntValue();
    }
    bool changeWindingOrder = true;
    if (FLOOR == type || RIGHT_WALL == type || FRONT_WALL == type || OUTSIDETOP == type || OUTSIDEBACK == type || OUTSIDELEFT == type)
    {
        changeWindingOrder = false;
    }

    Plane* p = new Plane(resource, corners[0], corners[1], corners[2], corners[3], changeWindingOrder);
    if (OUTSIDETOP == type ||
        OUTSIDEBOTTOM == type ||
        OUTSIDELEFT == type ||
        OUTSIDERIGHT == type ||
        OUTSIDEFRONT == type || 
        OUTSIDEBACK == type)
    {
        p->setColoms(25);
        p->setRows(25);
    }

    if (type == FLOOR || type == LEFT_WALL || type == BACK_WALL || type == OUTSIDETOP || type == OUTSIDERIGHT || type == OUTSIDEFRONT)
    {
        p->invertNormal();
    }

    const ShaderInstance shaderInstance = p->deserialise(element);
    PlanesMap::iterator pit = m_planes.find(type);
    if (pit != m_planes.end())
        pit->second.push_back(p);
    else
    {
        m_planes.insert(std::pair<int, Planes>(type, Planes(1, p)));
    }
    p->initialise(shaderInstance);
    GameResourceHelper gameResourceHelper(resource);
    GameObjectManager& gameObjectManager = gameResourceHelper.getWriteableResource().getGameObjectManager();
    gameObjectManager.addGameObject(p);
}

