#ifndef RotatingBlades_H
#define RotatingBlades_H

#include "..\SpaceSim\GameObject.h"
#include "face.h"
#include "vector3.h"

class Resource;

class RotatingBlades : public GameObject
{
public:
	RotatingBlades(Resource* resource) : GameObject(resource), m_rotatingblades1(resource, 50, 50, 10, 10, false), m_rotatingblades2(resource, 50, 50, 10, 10, false) {}
	~RotatingBlades(){}
	RotatingBlades(Resource* resource, const Vector3& position);
    void initialise(const ShaderInstance& shaderInstance, bool changeWindingOrder) 
    {
        m_rotatingblades1.initialise(shaderInstance, .0f, true , false, false, changeWindingOrder, false);
        m_rotatingblades2.initialise(shaderInstance, .0f, true , false, false, changeWindingOrder, false);
        m_active = true;

        //Super::initialise(shaderInstance);
    }

	
	
	int getNumVertices() const { return 0;}
    //-------------------------------------------------------------------------
    // @brief 
    //-------------------------------------------------------------------------
    const ShaderInstance deserialise( const tinyxml2::XMLElement* element );

    //-------------------------------------------------------------------------
    // @brief 
    //-------------------------------------------------------------------------
    virtual void update( RenderInstanceTree& renderInstances, float elapsedTime, const Input& input );

    //-------------------------------------------------------------------------
    // @brief 
    //-------------------------------------------------------------------------
    virtual void handleMessage( const Message& msg );

    HASH_ELEMENT_DEFINITION
protected:
private:
	Vector3 m_position;
	Face m_rotatingblades1;
	Face m_rotatingblades2;
	float m_angle;
};
#endif
