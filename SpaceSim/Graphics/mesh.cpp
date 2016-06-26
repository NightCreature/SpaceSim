#include "Graphics/mesh.h"
#include <fstream>
#include <iostream>
#include "Graphics/texture.h"
#include "Graphics/texturemanager.h"
#include "Application/BaseApplication.h"
#include "Graphics/DeviceManager.h"
#include "Core/Types/TypeHelpers.h"
#ifdef  _WIN32
#include <windows.h>
#endif//_WIN32



#include "assert.h"

namespace Mesh
{

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
CreatedModel CreateMesh(const CreationParams& params)
{
    CreatedModel mesh;

    GameResource& gameResource = *(GameResource*)params.m_resource;
    const ShaderInstance& shaderInstance = *(params.m_shaderInstance);

    mesh.model = new Model();

    if (mesh.model->getMeshData().empty())
    {
        VertexBuffer* vb = new VertexBuffer();
        IndexBuffer* ib = new IndexBuffer();
        mesh.model->getMeshData().push_back(new MeshGroup(vb, ib, shaderInstance));
        if (mesh.model->getMeshData()[0]->getShaderInstance().getMaterial().getEffect() == nullptr)
        {
            mesh.model->getMeshData()[0]->getShaderInstance().getMaterial().setEffect(gameResource.getEffectCache().getEffect("simple_effect.xml"));
        }

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

            mesh.boundingBox.enclose(params.m_vertices[counter]);
        }
        std::vector<unsigned int> texCoordDimensions;
        for (unsigned int texCoordCounter = 0; texCoordCounter < m_nummultitexcoords; ++texCoordCounter)
        {
            texCoordDimensions.push_back(2);
        }
        vertexData = vertexData - bufferSize;
        const Technique* technique = mesh.model->getMeshData()[0]->getShaderInstance().getMaterial().getEffect()->getTechnique("default");
        const VertexShader* shader = gameResource.getShaderCache().getVertexShader(technique->getVertexShader());
        assert(shader);
        vb->createBufferAndLayoutElements(gameResource.getDeviceManager(), bufferSize, vertexData, false, params.m_vertexDeclaration, shader->getShaderBlob());
        //vb->createVertexInputLayout(gameResource.getDeviceManager(), mesh.model->getMeshData()[0]->getShaderInstance().getMaterial().getEffect().getVertexShaderBlob());
        delete[] vertexData;

        ib->setNumberOfIndecis((unsigned int)params.m_indices.size());
        ib->createBuffer(gameResource.getDeviceManager(), (unsigned int)params.m_indices.size() * sizeof(unsigned int), (void*)&params.m_indices[0], false, D3D11_BIND_INDEX_BUFFER);
    }
    else
    {
        mesh.model->getMeshData()[0]->setShaderInstance(shaderInstance);
        if (mesh.model->getMeshData()[0]->getShaderInstance().getMaterial().getEffect() == nullptr)
        {
            mesh.model->getMeshData()[0]->getShaderInstance().getMaterial().setEffect(gameResource.getEffectCache().getEffect("simple_effect.fx"));
        }
    }

    mesh.model->getMeshData()[0]->getGeometryInstance().setPrimitiveType((unsigned int)D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


    return mesh;
}

void normalizeNormals(std::vector<Vector3>& normals)
{
    for (int i = 0; i < normals.size(); i++)
    {
        normals[i].normalize();
    }
}

}