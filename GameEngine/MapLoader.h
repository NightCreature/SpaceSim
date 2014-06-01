#ifndef MAP_H
#define MAP_H

#include <d3d11.h>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include "Model.h"
#include "texturemanager.h"
#include "texture.h"
#include "plane.h"
#include "bbox.h"
#include "player.h"
#include "vector3.h"
#include "player.h"
#include "..\SpaceSim\Input.h"
#include "..\SpaceSim\StringHelperFunctions.h"
#include <list>

class RenderInstance;
class Resource;

class MapContainer
{
public:
    void update(RenderInstanceTree& renderInstances, float elapsedTime, const Input& input) { UNUSEDPARAM(renderInstances);  UNUSEDPARAM(elapsedTime);  UNUSEDPARAM(input); }
    bool checkCollision(const Bbox& v, const Vector3& dir, Player& p, bool& special, Vector3& planenormal);
    bool checkLaserCollision(const Bbox& v, const Vector3& dir, bool& special, Vector3& planenormal, Player& p, bool lasertype = true);


    void insertWallElement(Plane* wallElement) { m_planes.push_back(wallElement); }
    void insertSpecial(GameObject* specialShape) { m_specials.push_back(specialShape); }
    const Bbox getBBox() const { return m_bbox; }
    bool coreDestroyed() const
    {
        //ShapeMap::const_iterator smcit = m_specials.find(CORE);
        //if (smcit == m_specials.end())
        //{
        //    MSG_TRACE_CHANNEL("ERROR", "Core was not found in the specials")
        //        return false;
        //}
        //return !smcit->second[0]->getActive();
        return false;
    }
private:
    typedef std::vector<Plane*> Planes;
    typedef std::vector<GameObject*> ShapeVector;
    void getCollisionNormal(Planes::const_iterator& it, Vector3& normal) { UNUSEDPARAM(it); UNUSEDPARAM(normal); }

    Planes m_planes;
    ShapeVector m_specials;
    Bbox m_bbox;
    float m_sequencestarttime;
};

class MapLoader
{
public:
	MapLoader() : m_exitshowing(false) {}
	~MapLoader();

	void cleanup();

	bool loadMap(Resource* resource, const std::string& filename);

    void reset();
protected:
private:
	//enums
	enum MAPTYPE
	{
		CEILING = 0,
		FLOOR,
		LEFT_WALL,
		RIGHT_WALL,
		FRONT_WALL,
		BACK_WALL,
		OUTSIDETOP,
		OUTSIDEBOTTOM,
		OUTSIDELEFT,
		OUTSIDERIGHT,
		OUTSIDEFRONT,
		OUTSIDEBACK,
		FORCEFIELDS,
		SLIDINGDOORS,
		ROTATINGBLADES,
		SWITCH,
		SWITCHALPHA,
		CORE,
		GUNTURRETS,
		SLIDINGDOORSALPHA,
		ROTATINGBLADESALPHA,

	};
	//typedefs
	typedef std::vector<Plane*> Planes;
	typedef std::map<int, Planes> PlanesMap;
	//functions
	void getCollisionNormal(PlanesMap::const_iterator& it, Vector3& normal);
    void readWallElement( Resource* resource, const tinyxml2::XMLElement* element );

    //Static const members
    static const unsigned int m_wallHash;
    static const unsigned int m_doorHash;
    static const unsigned int m_rotatingBladesHash;
    static const unsigned int m_switchHash;
    static const unsigned int m_coreHash;
    static const unsigned int m_forceFieldHash;

	//members
	std::string m_filename;
    PlanesMap m_planes;
	bool m_exitshowing;
};
#endif