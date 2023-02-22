#pragma once

#include "Math/vector3.h"
#include "Graphics/Model.h"
#include "Graphics/ShaderInstance.h"
#include "Graphics/VertexBuffer.h"

#include <vector>
#include <variant>

namespace MeshGroupCreator
{
    typedef std::vector<Vector3> TexCoords;

    typedef std::vector<TexCoords> MultiTexCoords;

    struct CreationParams : public Model::CreationParams
    {
        CommandList* m_commandList;
        ShaderInstance m_shaderInstance;
        Material mat;

        //Index data
        std::vector<unsigned int>	 m_indices;

        //Vertex Data
        VertexDeclarationDescriptor m_vertexDeclaration;
        std::vector<Vector3>		 m_vertices;
        std::vector<Vector3>		 m_normals;
        std::vector<Vector3>		 m_tangents;
        std::vector<Vector3>		 m_binormals;
        MultiTexCoords				 m_texcoords;
        size_t m_numvertices;
        size_t m_numnormals;
        size_t m_numtangents;
        size_t m_numtexcoords;

        size_t m_numindices;
        VertexDataStreams m_dataStream;
    };

    CreatedMeshGroup CreateMeshGroup(const CreationParams& params);
    void normalizeNormals(std::vector<Vector3>& normals);
};
