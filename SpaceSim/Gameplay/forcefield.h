#ifndef FORCEFIELD_H
#define FORCEFIELD_H

#include "Gameplay/GameObject.h"
#include "Gameplay/face.h"
#include "Math/vector3.h"

class ForceField : public GameObject
{
public:
	ForceField(Resource* resource) : GameObject(resource), m_texturespeed(0.0f)
    {
    }
	~ForceField(){}
	ForceField(Resource* resource, const Vector3& position);

    void initialise(bool changeWindingOrder);

	
	
	int getNumVertices() const {return 0;}
    void deserialise( const tinyxml2::XMLElement* element );
    virtual void update( float elapsedTime, const Input& input );
    virtual void handleMessage( const MessageSystem::Message& msg );

    HASH_ELEMENT_DEFINITION(ForceField)
protected:

	void OnDebugImguiInternal() override;
private:
    Material::MaterialParameters m_materialParameters;
	Vector3 m_position;
	float m_texturespeed;
};
#endif
