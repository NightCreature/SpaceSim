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
    typedef std::map<int, TexCoords> MultiTexCoords;

    struct CreationParams
    {
        ShaderInstance* shaderInstance;
        Resource* resource;

        int m_numvertices;
        int m_numindices;
        int m_numnormals;
        int m_numtexcoords;
    };

    struct CreatedMesh 
    {
        Model* model;
        Bbox boundingBox;
    };

    CreatedMesh CreateMesh(const CreationParams& params);
    void normalizeNormals(std::vector<Vector3>& normals);

};
#endif