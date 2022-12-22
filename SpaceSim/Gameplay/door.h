#ifndef DOOR_H
#define DOOR_H

#include "Gameplay/GameObject.h"
#include "Gameplay/face.h"
#include "Math/vector3.h"

class Door : public GameObject
{
public:
	Door(Resource* resource) : GameObject(resource)
    {
    }
	~Door(){}
	Door(Resource* resource, const Vector3& position);
    void initialise(const ShaderInstance& shaderInstance, bool changeWindingOrder);

	void move(float elapsedtime);

	int getNumVertices() const {return 0;}
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

    HASH_ELEMENT_DEFINITION(Door)
protected:
private:
    Material::MaterialParameters m_materialParameters;
	Vector3 m_position;
	float m_move;
	bool m_plus;
};

//Door as Entity
//MaterialCompoenent
//PositionComponent
//DoorAnimateComponent
#endif
