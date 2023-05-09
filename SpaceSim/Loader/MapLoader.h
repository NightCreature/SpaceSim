#ifndef MAP_H
#define MAP_H

#include <d3d11.h>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include "Graphics/Model.h"
#include "Graphics/texturemanager.h"
#include "Graphics/texture.h"
#include "Gameplay/plane.h"
#include "Gameplay/bbox.h"
#include "Gameplay/player.h"
#include "Math/vector3.h"
#include "Input/Input.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include <list>

class RenderInstance;
class Resource;

class MapContainer
{
public:
    void update(float elapsedTime, const Input& input) { UNUSEDPARAM(elapsedTime);  UNUSEDPARAM(input); }
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
    static constexpr size_t m_wallHash = "Wall"_hash;
    static constexpr size_t m_doorHash = "Door"_hash;
    static constexpr size_t m_rotatingBladesHash = "RotatingBlades"_hash;
    static constexpr size_t m_switchHash = "Switch"_hash;
    static constexpr size_t m_coreHash = "Core"_hash;
    static constexpr size_t m_forceFieldHash = "ForceField"_hash;

	//members
	std::string m_filename;
    PlanesMap m_planes;
	bool m_exitshowing;
};
#endif