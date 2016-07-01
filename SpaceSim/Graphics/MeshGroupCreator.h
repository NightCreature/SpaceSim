#pragma once

class MeshGroupCreator
{
public:
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
        int m_numvertices;
        int m_numindices;
        int m_numnormals;
        int m_numtangents;
        int m_numtexcoords;
    };

    CreatedMeshGroup CreateMeshGroup(const CreationParams& params);
    void normalizeNormals(std::vector<Vector3>& normals);
};
