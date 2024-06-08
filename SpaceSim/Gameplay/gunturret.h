#ifndef GUNTURRET_H
#define GUNTURRET_H

#include "Gameplay/GameObject.h"
#include "Math/vector3.h"
#include "Gameplay/laser.h"
#include "Gameplay/scorchmark.h"
#include "Loader/MapLoader.h"
#include <string>

namespace DebugGraphics
{
class DebugBox;
}

class GunTurret : public GameObject
{
public:
    typedef std::vector<Laser*> LaserList;
    typedef std::vector<ScorchMark*> ScorchMarkList;

	GunTurret(Resource* resource) : GameObject(resource) { m_lasergentime = 0; m_world.identity(); m_active = true; }
	GunTurret(Resource* resource, const Vector3& position, const Vector3& direction);
	~GunTurret();
    void initialise();
	void cleanup();

    virtual void update( float elapsedTime, const Input& input );
	

	void onHit();
	void fireLaser();
	void updateLasers(float elapsedtime/*, MapLoader& m_map, Player& p*/);
	void createScorchMark(const Vector3& pos, const Vector3 &normal);
    void deserialise( const tinyxml2::XMLElement* element );
    HASH_ELEMENT_DEFINITION(GunTurret)
protected:

	void OnDebugImguiInternal() override;
private:
	bool checkLaserCollisionPlayer(const Laser& l, Player& p);
	void transformBoundingBox();
	void eraseLaser(LaserList::iterator& it, Laser* l);

    ///-------------------------------------------------------------------------
    // @brief 
    ///-------------------------------------------------------------------------
    virtual void handleMessage( const MessageSystem::Message& msg );

    LaserList m_lasers;
    LaserList::iterator m_lasersit;
    ScorchMarkList m_scorchmarks;
    ScorchMarkList::iterator m_scorchit;
    Vector3 m_position;
	Vector3 m_direction;
	Vector3 m_center;
	Vector3 m_offset;
	float	m_laserspeed;
	float	m_lasergentime;
};
#endif