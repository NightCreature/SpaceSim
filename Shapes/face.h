#ifndef FACE_H
#define FACE_H

#include "Model.h"
#include <vector>
#include <map>

class Resource;
class Matrix44;

class Face : public Model
{
public:
	Face(Resource* resource);
    Face(Resource* resource, const Face& face) : 
        Super(resource),
        m_rows(face.m_rows),
        m_coloms(face.m_coloms),
        m_numberOfTexcoords(face.m_numberOfTexcoords),
        m_width(face.m_width),
        m_height(face.m_height),
        m_corridorwidth(face.m_corridorwidth),
        m_corridorheight(face.m_corridorheight),
        m_tesselate(face.m_tesselate)
        {
        }
	Face(Resource* resource, float width, float height, int nrxvertices, int nryvertices, bool tesselate = true);
	~Face();
    void initialise(const ShaderInstance& shaderInstance) { UNUSEDPARAM(shaderInstance); }
	void initialise(const ShaderInstance& shaderInstance, float fillvalue, bool fillx, bool filly, bool fillz, bool changeWindingOrder, bool invertNormal);

	int getRows() const{return m_rows;}
	int getColoms() const{return m_coloms;}	
	float getWidth() const{return m_width;}
	float getHeight() const{return m_height;}
    void setNumberOfTextureCoordinatesToGenerate(int numberOfTexcoords) { m_numberOfTexcoords = numberOfTexcoords; }

    const Face& operator= (const Face& face)
    {
        m_rows = face.m_rows;
        m_coloms = face.m_coloms;
        m_numberOfTexcoords = face.m_numberOfTexcoords;
        m_width = face.m_width; m_height = face.m_height;
        m_corridorwidth = face.m_corridorwidth;
        m_corridorheight = face.m_corridorheight;
        m_tesselate = face.m_tesselate;
        return *this;
    }

    static unsigned long long m_totalNumberOfVerts;
    static unsigned long long m_totalNumberOfPolygons;
protected:
private:
    void createVertexData( bool fillx, float fillvalue, bool filly, bool fillz, byte*& vertexData, bool invertNormal );
    void createIndexData( unsigned int*& indecis, bool changeWindingOrder );

	int   m_rows;
	int   m_coloms;
	int	  m_numberOfTexcoords;
	float m_width;
	float m_height;
	float m_corridorwidth;//Check
	float m_corridorheight;
	bool  m_tesselate;
};
#endif