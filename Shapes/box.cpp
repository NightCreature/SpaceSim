#include "box.h"
#include "texturemanager.h"
#include "windows.h"
#include "..\SpaceSim\BaseApplication.h"
#include "..\SpaceSim\EffectCache.h"
#include "..\SpaceSim\Types.h"
#include "..\SpaceSim\TypeHelpers.h"

Box::Box(Resource* resource) :
Super(resource),
m_lowerleft(Vector3::zero()),
m_upperright(Vector3::zero()),
m_dynamic(false),
m_gentexcoords(false)
{
    m_boundingBox = Bbox(m_lowerleft, m_upperright);
}

Box::Box(Resource* resource, const Vector3& lowerleft, const Vector3& upperright, bool dynamic, bool gentexcoords) :
Super(resource),
m_lowerleft(lowerleft),
m_upperright(upperright),
m_dynamic(dynamic),
m_gentexcoords(gentexcoords)
{
    m_boundingBox = Bbox(m_lowerleft, m_upperright);
}

Box::~Box()
{
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void Box::initialise(const ShaderInstance& shaderInstance)
{
    if (m_modelData.empty())
    {
        VertexBuffer* vb = new VertexBuffer();
        IndexBuffer* ib = new IndexBuffer();

        m_modelData.push_back(new MeshGroup(vb, ib, shaderInstance));
        if (m_modelData[0]->getShaderInstance().getMaterial().getEffect() == nullptr)
        {
            m_modelData[0]->getShaderInstance().getMaterial().setEffect(getGameResource().getEffectCache().getEffect("laser_effect.xml"));
        }

        std::vector<unsigned int> texCoordDim;
        if (m_gentexcoords)
        {
            texCoordDim.push_back(2);
        }

        unsigned int numberOfBytes = 6 * 4 * (3 + (m_gentexcoords ? 2 : 0)) * sizeof(float);
        char* data = new char[numberOfBytes]; //4 points with position and two 2D textureCoordinates * 6 faces for a box
        char* startOfData = data;
        //Front face
        *(float*)data = m_lowerleft.x(); data += sizeof(float);
        *(float*)data = m_lowerleft.y(); data += sizeof(float);
        *(float*)data = m_lowerleft.z(); data += sizeof(float);
        if (m_gentexcoords)
        {
            *(float*)data = 0.0f; data += sizeof(float);
            *(float*)data = 0.0f; data += sizeof(float);
        }
        *(float*)data = m_upperright.x(); data += sizeof(float);
        *(float*)data = m_lowerleft.y(); data += sizeof(float);
        *(float*)data = m_lowerleft.z(); data += sizeof(float);
        if (m_gentexcoords)
        {
            *(float*)data = 1.0f; data += sizeof(float);
            *(float*)data = 0.0f; data += sizeof(float);
        }
        *(float*)data = m_upperright.x(); data += sizeof(float);
        *(float*)data = m_upperright.y(); data += sizeof(float);
        *(float*)data = m_lowerleft.z(); data += sizeof(float);
        if (m_gentexcoords)
        {
            *(float*)data = 1.0f; data += sizeof(float);
            *(float*)data = 1.0f; data += sizeof(float);
        }
        *(float*)data = m_lowerleft.x(); data += sizeof(float);
        *(float*)data = m_upperright.y(); data += sizeof(float);
        *(float*)data = m_lowerleft.z(); data += sizeof(float);
        if (m_gentexcoords)
        {
            *(float*)data = 0.0f; data += sizeof(float);
            *(float*)data = 1.0f; data += sizeof(float);
        }
        //Back face
        *(float*)data = m_lowerleft.x(); data += sizeof(float);
        *(float*)data = m_lowerleft.y(); data += sizeof(float);
        *(float*)data = m_upperright.z(); data += sizeof(float);
        if (m_gentexcoords)
        {
            *(float*)data = 0.0f; data += sizeof(float);
            *(float*)data = 0.0f; data += sizeof(float);
        }
        *(float*)data = m_upperright.x(); data += sizeof(float);
        *(float*)data = m_lowerleft.y(); data += sizeof(float);
        *(float*)data = m_upperright.z(); data += sizeof(float);
        if (m_gentexcoords)
        {
            *(float*)data = 1.0f; data += sizeof(float);
            *(float*)data = 0.0f; data += sizeof(float);
        }
        *(float*)data = m_upperright.x(); data += sizeof(float);
        *(float*)data = m_upperright.y(); data += sizeof(float);
        *(float*)data = m_upperright.z(); data += sizeof(float);
        if (m_gentexcoords)
        {
            *(float*)data = 1.0f; data += sizeof(float);
            *(float*)data = 1.0f; data += sizeof(float);
        }
        *(float*)data = m_lowerleft.x(); data += sizeof(float);
        *(float*)data = m_upperright.y(); data += sizeof(float);
        *(float*)data = m_upperright.z(); data += sizeof(float);
        if (m_gentexcoords)
        {
            *(float*)data = 0.0f; data += sizeof(float);
            *(float*)data = 1.0f; data += sizeof(float);
        }
        //Left face
        *(float*)data = m_lowerleft.x(); data += sizeof(float);
        *(float*)data = m_lowerleft.y(); data += sizeof(float);
        *(float*)data = m_lowerleft.z(); data += sizeof(float);
        if (m_gentexcoords)
        {
            *(float*)data = 0.0f; data += sizeof(float);
            *(float*)data = 0.0f; data += sizeof(float);
        }
        *(float*)data = m_lowerleft.x(); data += sizeof(float);
        *(float*)data = m_lowerleft.y(); data += sizeof(float);
        *(float*)data = m_upperright.z(); data += sizeof(float);
        if (m_gentexcoords)
        {
            *(float*)data = 1.0f; data += sizeof(float);
            *(float*)data = 0.0f; data += sizeof(float);
        }
        *(float*)data = m_lowerleft.x(); data += sizeof(float);
        *(float*)data = m_upperright.y(); data += sizeof(float);
        *(float*)data = m_upperright.z(); data += sizeof(float);
        if (m_gentexcoords)
        {
            *(float*)data = 1.0f; data += sizeof(float);
            *(float*)data = 1.0f; data += sizeof(float);
        }
        *(float*)data = m_lowerleft.x(); data += sizeof(float);
        *(float*)data = m_upperright.y(); data += sizeof(float);
        *(float*)data = m_lowerleft.z(); data += sizeof(float);
        if (m_gentexcoords)
        {
            *(float*)data = 0.0f; data += sizeof(float);
            *(float*)data = 1.0f; data += sizeof(float);
        }
        //Right face
        *(float*)data = m_upperright.x(); data += sizeof(float);
        *(float*)data = m_lowerleft.y(); data += sizeof(float);
        *(float*)data = m_lowerleft.z(); data += sizeof(float);
        if (m_gentexcoords)
        {
            *(float*)data = 0.0f; data += sizeof(float);
            *(float*)data = 0.0f; data += sizeof(float);
        }
        *(float*)data = m_upperright.x(); data += sizeof(float);
        *(float*)data = m_lowerleft.y(); data += sizeof(float);
        *(float*)data = m_upperright.z(); data += sizeof(float);
        if (m_gentexcoords)
        {
            *(float*)data = 1.0f; data += sizeof(float);
            *(float*)data = 0.0f; data += sizeof(float);
        }
        *(float*)data = m_upperright.x(); data += sizeof(float);
        *(float*)data = m_upperright.y(); data += sizeof(float);
        *(float*)data = m_upperright.z(); data += sizeof(float);
        if (m_gentexcoords)
        {
            *(float*)data = 1.0f; data += sizeof(float);
            *(float*)data = 1.0f; data += sizeof(float);
        }
        *(float*)data = m_upperright.x(); data += sizeof(float);
        *(float*)data = m_upperright.y(); data += sizeof(float);
        *(float*)data = m_lowerleft.z(); data += sizeof(float);
        if (m_gentexcoords)
        {
            *(float*)data = 0.0f; data += sizeof(float);
            *(float*)data = 1.0f; data += sizeof(float);
        }
        //	//Top face
        *(float*)data = m_lowerleft.x(); data += sizeof(float);
        *(float*)data = m_upperright.y(); data += sizeof(float);
        *(float*)data = m_lowerleft.z(); data += sizeof(float);
        if (m_gentexcoords)
        {
            *(float*)data = 0.0f; data += sizeof(float);
            *(float*)data = 0.0f; data += sizeof(float);
        }
        *(float*)data = m_upperright.x(); data += sizeof(float);
        *(float*)data = m_upperright.y(); data += sizeof(float);
        *(float*)data = m_lowerleft.z(); data += sizeof(float);
        if (m_gentexcoords)
        {
            *(float*)data = 1.0f; data += sizeof(float);
            *(float*)data = 0.0f; data += sizeof(float);
        }
        *(float*)data = m_upperright.x(); data += sizeof(float);
        *(float*)data = m_upperright.y(); data += sizeof(float);
        *(float*)data = m_upperright.z(); data += sizeof(float);
        if (m_gentexcoords)
        {
            *(float*)data = 1.0f; data += sizeof(float);
            *(float*)data = 1.0f; data += sizeof(float);
        }
        *(float*)data = m_lowerleft.x(); data += sizeof(float);
        *(float*)data = m_upperright.y(); data += sizeof(float);
        *(float*)data = m_upperright.z(); data += sizeof(float);
        if (m_gentexcoords)
        {
            *(float*)data = 0.0f; data += sizeof(float);
            *(float*)data = 1.0f; data += sizeof(float);
        }
        //Bottom face
        *(float*)data = m_lowerleft.x(); data += sizeof(float);
        *(float*)data = m_lowerleft.y(); data += sizeof(float);
        *(float*)data = m_lowerleft.z(); data += sizeof(float);
        if (m_gentexcoords)
        {
            *(float*)data = 0.0f; data += sizeof(float);
            *(float*)data = 0.0f; data += sizeof(float);
        }
        *(float*)data = m_upperright.x(); data += sizeof(float);
        *(float*)data = m_lowerleft.y(); data += sizeof(float);
        *(float*)data = m_lowerleft.z(); data += sizeof(float);
        if (m_gentexcoords)
        {
            *(float*)data = 1.0f; data += sizeof(float);
            *(float*)data = 0.0f; data += sizeof(float);
        }
        *(float*)data = m_upperright.x(); data += sizeof(float);
        *(float*)data = m_lowerleft.y(); data += sizeof(float);
        *(float*)data = m_upperright.z(); data += sizeof(float);
        if (m_gentexcoords)
        {
            *(float*)data = 1.0f; data += sizeof(float);
            *(float*)data = 1.0f; data += sizeof(float);
        }
        *(float*)data = m_lowerleft.x(); data += sizeof(float);
        *(float*)data = m_lowerleft.y(); data += sizeof(float);
        *(float*)data = m_upperright.z(); data += sizeof(float);
        if (m_gentexcoords)
        {
            *(float*)data = 0.0f; data += sizeof(float);
            *(float*)data = 1.0f; data += sizeof(float);
        }

        const Technique* technique = m_modelData[0]->getShaderInstance().getMaterial().getEffect()->getTechnique("default");
        VertexDecalartionDesctriptor vertexDesc;
        vertexDesc.textureCoordinateDimensions = texCoordDim;
        vb->createBufferAndLayoutElements(getGameResource().getDeviceManager(), numberOfBytes, (void*)startOfData, m_dynamic, vertexDesc, technique->getVertexShader()->getShaderBlob());
        delete [] startOfData;
        data = nullptr;
        startOfData = nullptr;
        unsigned int indexData[] = 
        {
            0, 1, 2, 2, 3, 0, //FF
            4, 5, 6, 6, 7, 4, //BF
            8, 9, 10, 10, 11, 8, //SF1
            12, 13, 14, 14, 15, 12, //SF2
            16, 17, 18, 18, 19, 16, //VF1
            20, 21, 22, 22, 23, 20 //VF2
        };

        ib->createBuffer(getGameResource().getDeviceManager(), sizeof(indexData), (void*)&indexData[0], false, D3D11_BIND_INDEX_BUFFER);
        ib->setNumberOfIndecis(36);
    }
    else
    {
        if (m_modelData[0]->getShaderInstance().getMaterial().getEffect() == nullptr)
        {
            m_modelData[0]->getShaderInstance().getMaterial().setEffect(getGameResource().getEffectCache().getEffect("laser_effect.xml"));
        }
    }    

    m_modelData[0]->getGeometryInstance().setPrimitiveType((unsigned int)D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
