#include <Graphics/MeshGroupCreator.h>

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
CreatedMeshGroup MeshGroupCreator::CreateMeshGroup(const CreationParams& params)
{
    CreatedMeshGroup meshGroup;

    VertexBuffer* vb = new VertexBuffer();
    IndexBuffer* ib = new IndexBuffer();

    unsigned int m_nummultitexcoords = 1; //HACK FIX THIS
    unsigned int bufferSize = 0;
    bufferSize += sizeof(float) * 3 * (unsigned int)params.m_vertices.size();
    bufferSize += sizeof(float) * 3 * (unsigned int)params.m_normals.size();
    bufferSize += sizeof(float) * 2 * m_nummultitexcoords * (unsigned int)params.m_vertices.size();//(unsigned int)params.m_texcoords[0].size();
    char* vertexData = new char[bufferSize];
    for (unsigned int counter = 0; counter < params.m_vertices.size(); ++counter)
    {
        *(float*)vertexData = params.m_vertices[counter].x(); vertexData += sizeof(float);
        *(float*)vertexData = params.m_vertices[counter].y(); vertexData += sizeof(float);
        *(float*)vertexData = params.m_vertices[counter].z(); vertexData += sizeof(float);
        *(float*)vertexData = params.m_normals[counter].x(); vertexData += sizeof(float);
        *(float*)vertexData = params.m_normals[counter].y(); vertexData += sizeof(float);
        *(float*)vertexData = params.m_normals[counter].z(); vertexData += sizeof(float);
        for (unsigned int texCoordCounter = 0; texCoordCounter < m_nummultitexcoords; ++texCoordCounter)
        {
            if (params.m_texcoords.size() > 0)
            {
                const TexCoords& texCoords = params.m_texcoords[texCoordCounter];
                if (texCoords.size() > 0)
                {
                    *(float*)vertexData = texCoords[counter].x(); vertexData += sizeof(float);
                    *(float*)vertexData = texCoords[counter].y(); vertexData += sizeof(float);
                }
                else
                {

                    *(float*)vertexData = 0.0f; vertexData += sizeof(float);
                    *(float*)vertexData = 0.0f; vertexData += sizeof(float);
                }
            }
            else
            {

                *(float*)vertexData = 0.0f; vertexData += sizeof(float);
                *(float*)vertexData = 0.0f; vertexData += sizeof(float);
            }
        }

        meshGroup.boundingBox.enclose(params.m_vertices[counter]);
    }
    std::vector<unsigned int> texCoordDimensions;
    for (unsigned int texCoordCounter = 0; texCoordCounter < m_nummultitexcoords; ++texCoordCounter)
    {
        texCoordDimensions.push_back(2);
    }
    vertexData = vertexData - bufferSize;
    const Technique* technique = params.m_shaderInstance.getMaterial().getEffect()->getTechnique("default");
    GameResource gameResource(params.m_resource);
    const VertexShader* shader = gameResource.getShaderCache().getVertexShader(technique->getVertexShader());
    assert(shader);
    vb->createBufferAndLayoutElements(gameResource.getDeviceManager(), bufferSize, vertexData, false, params.m_vertexDeclaration, shader->getShaderBlob());
    //vb->createVertexInputLayout(gameResource.getDeviceManager(), mesh.model->getMeshData()[0]->getShaderInstance().getMaterial().getEffect().getVertexShaderBlob());
    delete[] vertexData;

    ib->setNumberOfIndecis((unsigned int)params.m_indices.size());
    ib->createBuffer(gameResource.getDeviceManager(), (unsigned int)params.m_indices.size() * sizeof(unsigned int), (void*)&params.m_indices[0], false, D3D11_BIND_INDEX_BUFFER);

    
    meshGroup.meshGroup = new MeshGroup(vb, ib, params.m_shaderInstance);
    return meshGroup;
}
