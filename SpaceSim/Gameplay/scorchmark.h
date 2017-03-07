#ifndef SCORCHMARK_H
#define SCORCHMARK_H

#include "Math/vector3.h"
#include "Math/matrix44.h"
#include "Gameplay/square.h"
#include "Gameplay/GameObject.h"

class Resource;

class ScorchMark : public GameObject
{
public:
	ScorchMark(Resource* resource) : GameObject(resource) {}
	ScorchMark(Resource* resource, const Vector3& position, const Vector3& normal, float lifetime);
	~ScorchMark() {}
    void initialise(const ShaderInstance& shaderInstance);
    virtual const ShaderInstance deserialise( const tinyxml2::XMLElement* node );

    virtual void update( RenderInstanceTree& renderInstances, float elapsedTime, const Input& input );
    float getLifetime() const {return m_lifetime;}
protected:
private:
	void create();

    //-------------------------------------------------------------------------
    // @brief 
    //-------------------------------------------------------------------------
    virtual void handleMessage( const MessageSystem::Message& msg );

	Vector3 m_position;
	Vector3 m_normal;
	float	m_lifetime;
};
#endif