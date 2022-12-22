#ifndef PLANE_H
#define PLANE_H

#include "Gameplay/GameObject.h"
#include "Math/vector3.h"
#include "Gameplay/face.h"

#include "Gameplay/EntityManager.h"

#include <vector>
#include <sstream>

class Plane : public GameObject
{
public:
	Plane(Resource* resource) : GameObject(resource) 
    {
        std::stringstream str("");
        str << "plane_"<< m_planeCount;
        ++m_planeCount;
        m_name = str.str();
        m_invertNormal = false;

       // e = getWriteableResource().getEntityManager().create();
    }
	Plane(Resource* resource, const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, bool changeWindingOrder);
	~Plane() {}

    void initialise(const ShaderInstance& shaderInstance);
    virtual const ShaderInstance deserialise( const tinyxml2::XMLElement* node );
    virtual void update( RenderInstanceTree& renderInstances, float elapsedTime, const Input& input );

	void transform();

	void setRows(int rows) {m_rows = rows;}
	void setColoms(int coloms) {m_coloms = coloms;}
    void invertNormal();
protected:
private:
	void translateBbox(float xt, float yt, float zt);
	void rotateBbox(float angle, float xr, float yr, float zr);

    virtual void handleMessage( const MessageSystem::Message& msg );
    
	float m_widthstartpos;
	float m_widthendpos;
	float m_heightstartpos;
	float m_heightendpos;
	float m_fillvalue;
	int m_rows;
    int m_coloms;
    bool  m_fillx;
    bool  m_filly;
    bool  m_fillz;
    bool m_invertNormal;
    bool m_changeWindingOrder;
    static int m_planeCount;

    Material::MaterialParameters m_materialParameters;
};
#endif