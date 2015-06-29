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
//Doesnt need to be  class only generation fucntion is enough
namespace Mesh
{

    typedef std::vector<Vector2> TexCoords;
    typedef std::vector<TexCoords> MultiTexCoords;

    struct CreationParams : public Model::CreationParams
    {
        std::vector<Vector3>		 m_vertices;
        std::vector<Vector3>		 m_normals;
        std::vector<unsigned int>	 m_indices;
        MultiTexCoords				 m_texcoords;
        int m_numvertices;
        int m_numindices;
        int m_numnormals;
        int m_numtexcoords;
    };

    CreatedModel CreateMesh(const CreationParams& params);
    void normalizeNormals(std::vector<Vector3>& normals);

};
#endif