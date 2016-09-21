#ifndef MESH_H
#define MESH_H

#include "Math/vector3.h"
#include "Math/vector2.h"
#include "Math/matrix44.h"
#include "Graphics/material.h"
#include "Graphics/Model.h"
#include "Graphics/color.h"
#include "Gameplay/bbox.h"
#include "Graphics/MeshGroupCreator.h"
#include "Graphics/VertexBuffer.h"
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

    typedef std::vector<Vector3> TexCoords;
    typedef std::vector<TexCoords> MultiTexCoords;

    struct CreationParams : public Model::CreationParams
    {
        std::vector<CreatedMeshGroup> m_meshGroups;
    };

    CreatedModel CreateMesh(const CreationParams& params);
    void normalizeNormals(std::vector<Vector3>& normals);

};
#endif