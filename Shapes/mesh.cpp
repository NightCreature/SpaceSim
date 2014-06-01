#include "mesh.h"
#include <fstream>
#include <iostream>
#include "texture.h"
#include "texturemanager.h"
#include "..\SpaceSim\BaseApplication.h"
#include "..\SpaceSim\DeviceManager.h"
#include "..\SpaceSim\TypeHelpers.h"
#ifdef  _WIN32
#include <windows.h>
#endif//_WIN32



#include "assert.h"



Mesh::Mesh(Resource* resource) :
Super(resource),
m_numvertices(0),
m_numindices(0),
m_numnormals(0),
m_numtexcoords(0)
{
}

Mesh::~Mesh()
{
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void Mesh::initialise(const ShaderInstance& shaderInstance)
{
    if (m_modelData.empty())
    {
        VertexBuffer* vb = new VertexBuffer();
        IndexBuffer* ib = new IndexBuffer();
        m_modelData.push_back(new MeshGroup(vb, ib, shaderInstance));
        if (m_modelData[0]->getShaderInstance().getMaterial().getEffect() == nullptr)
        {
            m_modelData[0]->getShaderInstance().getMaterial().setEffect(getGameResource().getEffectCache().getEffect("simple_effect.xml"));
        }

        m_nummultitexcoords = 1; //HACK FIX THIS
        unsigned int bufferSize = 0;
        bufferSize += sizeof(float) * 3 * (unsigned int)m_vertices.size();
        bufferSize += sizeof(float) * 3 * (unsigned int)m_normals.size();
        bufferSize += sizeof(float) * 2 * m_nummultitexcoords * (unsigned int)m_vertices.size();//(unsigned int)m_texcoords[0].size();
        char* vertexData = new char[bufferSize];
        for (unsigned int counter = 0; counter < m_vertices.size(); ++counter)
        {
            *(float*)vertexData = m_vertices[counter].x(); vertexData += sizeof(float);
            *(float*)vertexData = m_vertices[counter].y(); vertexData += sizeof(float);
            *(float*)vertexData = m_vertices[counter].z(); vertexData += sizeof(float);
            *(float*)vertexData = m_normals[counter].x(); vertexData += sizeof(float);
            *(float*)vertexData = m_normals[counter].y(); vertexData += sizeof(float);
            *(float*)vertexData = m_normals[counter].z(); vertexData += sizeof(float);
            for (unsigned int texCoordCounter = 0; texCoordCounter < (unsigned int)m_nummultitexcoords; ++texCoordCounter)
            {
                TexCoords& texCoords = m_texcoords[texCoordCounter];
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

            m_boundingBox.enclose(m_vertices[counter]);
        }
        std::vector<unsigned int> texCoordDimensions;
        for (unsigned int texCoordCounter = 0; texCoordCounter < (unsigned int)m_nummultitexcoords; ++texCoordCounter)
        {
            texCoordDimensions.push_back(2);
        }
        vertexData = vertexData - bufferSize;
        const Technique* technique = m_modelData[0]->getShaderInstance().getMaterial().getEffect()->getTechnique("default");
        VertexDecalartionDesctriptor vertexDesc;
        vertexDesc.normal = true;
        vertexDesc.textureCoordinateDimensions = texCoordDimensions;
        vb->createBufferAndLayoutElements(getGameResource().getDeviceManager(), bufferSize, vertexData, false, vertexDesc, technique->getVertexShader()->getShaderBlob());
        //vb->createVertexInputLayout(getGameResource().getDeviceManager(), m_modelData[0]->getShaderInstance().getMaterial().getEffect().getVertexShaderBlob());
        delete [] vertexData;

        ib->setNumberOfIndecis( (unsigned int)m_indices.size() );
        ib->createBuffer(getGameResource().getDeviceManager(), (unsigned int)m_indices.size() * sizeof(unsigned int), (void*)&m_indices[0], false, D3D11_BIND_INDEX_BUFFER);
         
        m_vertices.clear();
        m_normals.clear();
        m_indices.clear();
        m_texcoords.clear();
    }
    else
    {
        m_modelData[0]->setShaderInstance( shaderInstance );
        if (m_modelData[0]->getShaderInstance().getMaterial().getEffect() == nullptr)
        {
            m_modelData[0]->getShaderInstance().getMaterial().setEffect(getGameResource().getEffectCache().getEffect("simple_effect.fx"));
        }
    }

    m_modelData[0]->getGeometryInstance().setPrimitiveType((unsigned int)D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Mesh::normalizeNormals()
{
    for (int i = 0; i < m_numnormals; i++)
    {
        m_normals[i].normalize();
    }
}

