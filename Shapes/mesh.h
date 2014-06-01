#ifndef MESH_H
#define MESH_H

#include "vector3.h"
#include "vector2.h"
#include "matrix44.h"
#include "material.h"
#include "Model.h"
#include "color.h"
#include "bbox.h"
#include <string>
#include <vector>
#include <map>

enum meshdatatype 
{
	vertices = 0,
	normals,
	texcoords,
	indices,
};

class Mesh : public Model
{
public:
	Mesh(Resource* resource);
	~Mesh();

    void initialise(const ShaderInstance& shaderInstance);

	int getNumVertices() const {return (int)m_vertices.size();}
	int getNumIndices() const {return (int)m_indices.size();}
	int getNumNormals() const {return (int)m_normals.size();}
	int getNumTexCoords() const { return (int)m_texcoords.size();}
	const std::vector<Vector3>& getVertices() const {return m_vertices;}
	const std::vector<Vector3>& getNormals() const {return m_normals;}
	const std::vector<Vector2>& getTexCoords(int stage) {return m_texcoords[stage];}
	const std::vector<unsigned int>& getIndices() const {return m_indices;}

    std::vector<Vector3>& getVertices() {return m_vertices;}
    std::vector<Vector3>& getNormals() {return m_normals;}
    std::vector<unsigned int>& getIndices() {return m_indices;}
    std::map<int, std::vector<Vector2> > & getTexCoords() {return m_texcoords;}

    void normalizeNormals();
protected:
private:
	typedef std::vector<Vector2> TexCoords;
	typedef std::map<int, TexCoords> MultiTexCoords;

	int							 m_numvertices;
	int							 m_numindices;
	int							 m_numnormals;
	int							 m_numtexcoords;
	int							 m_nummultitexcoords;
	std::vector<Vector3>		 m_vertices;
	std::vector<Vector3>		 m_normals;
	std::vector<unsigned int>	 m_indices;
	MultiTexCoords				 m_texcoords;
};
#endif