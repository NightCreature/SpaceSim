#ifndef FORCEFIELD_H
#define FORCEFIELD_H

#include "..\SpaceSim\GameObject.h"
#include "face.h"
#include "vector3.h"

class ForceField : public GameObject
{
public:
	ForceField(Resource* resource) : GameObject(resource), m_texturespeed(0.0f)
    {
        m_drawableObject = nullptr;
    }
	~ForceField(){}
	ForceField(Resource* resource, const Vector3& position);

    void initialise(const ShaderInstance& shaderInstance, bool changeWindingOrder);

	
	
	int getNumVertices() const {return 0;}
    const ShaderInstance deserialise( const tinyxml2::XMLElement* element );
    virtual void update( RenderInstanceTree& renderInstances, float elapsedTime, const Input& input );
    virtual void handleMessage( const Message& msg );

    HASH_ELEMENT_DEFINITION
protected:
private:
	Vector3 m_position;
	float m_texturespeed;
};
#endif
