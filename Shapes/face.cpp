#include "face.h"
#include "matrixmath.h"
#include "settingsparser.h"
#include "..\SpaceSim\DeviceManager.h"
#include <math.h>
#include <assert.h>
#include <Windows.h>
#include <directxmath.h>

#include "..\SpaceSim\BaseApplication.h" //Hack for now
#include "..\SpaceSim\TypeHelpers.h" //Hack for now
#include "..\SpaceSim\ShaderInstance.h"
#include "..\SpaceSim\StringHelperFunctions.h"


unsigned long long Face::m_totalNumberOfVerts = 0;
unsigned long long Face::m_totalNumberOfPolygons = 0;

Face::Face(Resource* resource):
Super(resource)
{
	m_width = 0;
	m_height = 0;
	m_numberOfTexcoords = 1;
}

Face::Face(Resource* resource, float width, float height, int nrxvertices, int nryvertices, bool tesselate):
Super(resource)
{
	assert(nrxvertices != 1 && nryvertices != 1);
	m_width = width;
	m_height = height;
	m_rows = nrxvertices;
	m_coloms = nryvertices;
	m_tesselate = tesselate;
	m_numberOfTexcoords = 1;


    m_corridorwidth = 50.0f;
    m_corridorheight = 50.0f;
}

Face::~Face()
{
}

void Face::initialise(const ShaderInstance& shaderInstance, float fillvalue, bool fillx, bool filly, bool fillz, bool changeWindingOrder, bool invertNormal)
{
    if (m_modelData.empty())
    {
        MeshGroup* group = new MeshGroup(new VertexBuffer(), new IndexBuffer(), shaderInstance); 
        m_modelData.push_back(group);
        m_modelData[0]->setShaderInstance( shaderInstance );
        if (m_modelData[0]->getShaderInstance().getMaterial().getEffect() == nullptr )
        {
            m_modelData[0]->getShaderInstance().getMaterial().setEffect(getGameResource().getEffectCache().getEffect("simple_effect.xml"));
        }

        VertexBuffer* vb = m_modelData[0]->getGeometryInstance().getVB();
        IndexBuffer* ib = m_modelData[0]->getGeometryInstance().getIB();
        
        std::vector<unsigned int> texCoordDimensions;
        for (int texCoordCounter = 0; texCoordCounter < m_numberOfTexcoords; ++texCoordCounter)
        {
            texCoordDimensions.push_back(2);
        }

        unsigned int bufferSize = 0;
        if ((m_height / m_corridorheight) > 1 && m_tesselate)
        {
            m_rows *= (int)(m_height / m_corridorheight);
        }
        if ((m_width / m_corridorwidth) > 1  && m_tesselate)
        {
            m_coloms *= (int)(m_width / m_corridorheight);
        }
        unsigned int numberOfVerts = m_rows * m_coloms;
        m_totalNumberOfVerts += numberOfVerts;
        bufferSize += sizeof(float) * 3 * numberOfVerts;
        bufferSize += sizeof(float) * 3 * numberOfVerts;
        bufferSize += sizeof(float) * 2 * m_numberOfTexcoords * numberOfVerts;
        byte* vertexData = new byte[bufferSize];
        byte* startOfVertexArray = vertexData;
        createVertexData( fillx, fillvalue, filly, fillz, vertexData, invertNormal );

        //Move pointer to start of vertex array  
        const Technique* technique = m_modelData[0]->getShaderInstance().getMaterial().getEffect()->getTechnique("default");
        VertexDecalartionDesctriptor vertexDesc;
        vertexDesc.normal = true;
        vertexDesc.textureCoordinateDimensions = texCoordDimensions;
        const VertexShader* shader = getGameResource().getShaderCache().getVertexShader(technique->getVertexShader());
        assert(shader);
        if (!vb->createBufferAndLayoutElements(getGameResource().getDeviceManager(), bufferSize, startOfVertexArray, false, vertexDesc, shader->getShaderBlob()))
        {
            MSG_TRACE_CHANNEL("VERTEXBUFFER_ERROR", "Failed to create VB!");
            assert(false);
        }

        delete [] startOfVertexArray; //cleanup
        vertexData = nullptr;
        startOfVertexArray = nullptr;

        unsigned int numberOfIndecis = (m_rows-1) * (m_coloms-1) * 6;
        ib->setNumberOfIndecis( numberOfIndecis );
        unsigned int* indecis = new unsigned int[numberOfIndecis];
        unsigned int* startOfIndexData = indecis;
        createIndexData(indecis, changeWindingOrder);

        ib->createBuffer(getGameResource().getDeviceManager(), numberOfIndecis * sizeof(unsigned int), (void*)startOfIndexData, false, D3D11_BIND_INDEX_BUFFER);
        delete [] startOfIndexData; //cleanup
        indecis = nullptr;
        startOfIndexData = nullptr;
        m_totalNumberOfPolygons += numberOfIndecis / 3;

        vb = nullptr;
        ib = nullptr;
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

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void Face::createVertexData( bool fillx, float fillvalue, bool filly, bool fillz, byte*& vertexData, bool invertNormal )
{
    if (0 == m_rows || 0 == m_coloms)
        return;
    float heightpart = m_height / ((float)m_rows - 1);
    if ((m_height / m_corridorheight) > 1 && m_tesselate)
    {
        heightpart = m_height / ((float)m_rows - 1);
    }
    float widthpart = m_width / ((float)m_coloms - 1);
    if ((m_width / m_corridorwidth) > 1  && m_tesselate)
    {
        widthpart = m_width / ((float)m_coloms - 1);
    }
    Vector3 v;
    Vector3 normal;
    if (fillx)
    {
        normal = Vector3::xAxis();
    }
    else if(filly)
    {
        normal = Vector3::yAxis();
    }
    else if(fillz)
    {
        normal = Vector3::zAxis();
    }

    if (invertNormal)
    {
        normal = -normal;
    }

    Vector2 texcoord;
    for (int i = 0; i < m_rows; i++)
    {
        for (int j = 0; j < m_coloms; j++)
        {
            if (fillx)				
                v = Vector3(fillvalue, i * heightpart, j * widthpart);
            if (filly)
                v = Vector3(i * heightpart, fillvalue, j * widthpart);
            if (fillz)
                v = Vector3(i * heightpart, j * widthpart, fillvalue);
            *(float*)vertexData = v.x(); vertexData += sizeof(float);
            *(float*)vertexData = v.y(); vertexData += sizeof(float);
            *(float*)vertexData = v.z(); vertexData += sizeof(float);

            m_boundingBox.enclose(v);

            *(float*)vertexData = normal.x(); vertexData += sizeof(float);
            *(float*)vertexData = normal.y(); vertexData += sizeof(float);
            *(float*)vertexData = normal.z(); vertexData += sizeof(float);

            float uPart = 1 / (float)(m_coloms - 1); //Minus one as index start at zero still want one at maxEle - 1
            float vPart = 1 / (float)(m_rows - 1);
            if ((m_width / m_corridorwidth) > 1 && m_tesselate)
                uPart = uPart * (m_width / m_corridorwidth);
            if ((m_height / m_corridorheight) > 1 && m_tesselate)
                vPart = vPart * (m_height / m_corridorheight);
            texcoord = Vector2(j * uPart, i * vPart);
            for (int k = 0; k < m_numberOfTexcoords; k++)
            {
                *(float*)vertexData = 1.0f - texcoord.x(); vertexData += sizeof(float);
                *(float*)vertexData = 1.0f - texcoord.y(); vertexData += sizeof(float);
            }
        }
    }
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void Face::createIndexData( unsigned int*& indecis, bool changeWindingOrder )
{
    for (int i = 0; i < m_rows-1; i++)
    {
        for (int j = 0; j < m_coloms-1; j++)
        {
            unsigned int indexvertex = (i * m_coloms) + j;
            if (changeWindingOrder)
            {
                *indecis = indexvertex + m_coloms + 1; ++indecis;
                *indecis = indexvertex + m_coloms; ++indecis;
                *indecis = indexvertex; ++indecis;

                *indecis = indexvertex + 1; ++indecis;
                *indecis = indexvertex + m_coloms + 1; ++indecis;
                *indecis = indexvertex; ++indecis;
            }
            else
            {
                *indecis = indexvertex; ++indecis;
                *indecis = indexvertex + m_coloms; ++indecis;
                *indecis = indexvertex + m_coloms + 1; ++indecis;

                *indecis = indexvertex; ++indecis;
                *indecis = indexvertex + m_coloms + 1; ++indecis;
                *indecis = indexvertex + 1; ++indecis;
            }
        }
    }
}
