#pragma once

#include "Math/vector3.h"
#include "Graphics/Model.h"
#include "Graphics/ShaderInstance.h"
#include "Graphics/VertexBuffer.h"

#include <vector>

namespace MeshGroupCreator
{
    typedef std::vector<Vector3> TexCoords;
    typedef std::vector<TexCoords> MultiTexCoords;

    struct CreationParams : public Model::CreationParams
    {
        ShaderInstance m_shaderInstance;
        VertexDecalartionDesctriptor m_vertexDeclaration;
        std::vector<Vector3>		 m_vertices;
        std::vector<Vector3>		 m_normals;
        std::vector<Vector3>		 m_tangents;
        std::vector<Vector3>		 m_binormals;
        std::vector<unsigned int>	 m_indices;
        MultiTexCoords				 m_texcoords;
        size_t m_numvertices;
        size_t m_numindices;
        size_t m_numnormals;
        size_t m_numtangents;
        size_t m_numtexcoords;
    };

    CreatedMeshGroup CreateMeshGroup(const CreationParams& params);
    void normalizeNormals(std::vector<Vector3>& normals);
};
