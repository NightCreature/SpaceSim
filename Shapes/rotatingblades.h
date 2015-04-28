#ifndef RotatingBlades_H
#define RotatingBlades_H

#include "..\SpaceSim\GameObject.h"
#include "face.h"
#include "vector3.h"

class Resource;

class RotatingBlades : public GameObject
{
public:
	RotatingBlades(Resource* resource) : GameObject(resource) {}
	~RotatingBlades(){}
	RotatingBlades(Resource* resource, const Vector3& position);
    void initialise(const ShaderInstance& shaderInstance, bool changeWindingOrder) 
    {
        Face::CreationParams params;
        params.shaderInstance = &shaderInstance;
        params.resource = m_resource;
        params.fillvalue = 0.0f;
        params.fillx = true;
        params.changeWindingOrder = changeWindingOrder;
        Face::CreatedFace face = Face::CreateFace(params);
        Face::CreatedFace face2 = Face::CreateFace(params);
        m_rotatingblades1 = face.model;
        m_rotatingblades2 = face2.model;
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
	Model* m_rotatingblades1;
    Model* m_rotatingblades2;
	float m_angle;
};
#endif
