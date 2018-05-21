#ifndef SWITCH_H
#define SWITCH_H

#include "Gameplay/GameObject.h"
#include "Math/vector3.h"
#include "Gameplay/face.h"
#include "Graphics/mesh.h"
#include <vector>

class Resource;
namespace DebugGraphics
{
class DebugBox;
}

class Switch : public GameObject
{
public:
	Switch(Resource* resource) : GameObject(resource) {}
	Switch(Resource* resource, Vector3 position, GameObject* associatedspecial);
	~Switch() {} //Only have a reference to the model
	void initialise(const ShaderInstance& shaderInstance);

	

	void onHit();
	
	void setMaterial1(const Material& mat) {m_mat1 = mat;}
	void setMaterial2(const Material& mat) {m_mat2 = mat;}
	const Material getMaterial1() {return m_mat1;}
	const Material getMaterial2() {return m_mat2;}

    const ShaderInstance deserialise( const tinyxml2::XMLElement* element );

    ///-------------------------------------------------------------------------
    // @brief 
    ///-------------------------------------------------------------------------
    virtual void update( RenderInstanceTree& renderInstances, float elapsedTime, const Input& input );

    ///-------------------------------------------------------------------------
    // @brief 
    ///-------------------------------------------------------------------------
    virtual void handleMessage( const MessageSystem::Message& msg );

    HASH_ELEMENT_DEFINITION(Switch)
protected:
private:
	bool m_activemat;
	float m_prevtime;
	Material m_mat1;
	Material m_mat2;
	GameObject* m_special;
	Vector3 m_position;
};
#endif