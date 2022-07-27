#ifndef CORE_H
#define CORE_H
#include "Gameplay/GameObject.h"

class Resource;

class Core : public GameObject
{
public:
    Core(Resource* resource);
	Core(Resource* resource, const Vector3& position, float radius, int slices, int stacks, int ffstrength);
	~Core() {}

    void initialise(const ShaderInstance& shaderInstance);

	void onHit();

	int getFFStrength() const {return m_ffstrength;}
	Vector3 getPosition() const {return m_position;}
    ///-------------------------------------------------------------------------
    // @brief 
    ///-------------------------------------------------------------------------
    const ShaderInstance deserialise( const tinyxml2::XMLElement* element );

    ///-------------------------------------------------------------------------
    // @brief 
    ///-------------------------------------------------------------------------
    virtual void update( RenderInstanceTree& renderInstances, float elapsedTime, const Input& input );

    ///-------------------------------------------------------------------------
    // @brief 
    ///-------------------------------------------------------------------------
    virtual void handleMessage( const MessageSystem::Message& msg );

    HASH_ELEMENT_DEFINITION(Core)
protected:
private:
	Vector3		m_position;
	//GLUquadric* m_core;
	// 
	//GLUquadric* m_coreforcefield;
	float		m_radius;
	float		m_angle;
	float		m_ffangle;
	float		m_timepassed;
	int			m_slices;
	int			m_stacks;
	int			m_ffstrength;
	bool		m_hit;
	Material	m_forcefieldmatnormal;
	Material	m_forcefieldmatglowing;
};
#endif