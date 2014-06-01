#include "square.h"
#include "vector4.h"
#include "matrix44.h"
#include "..\\SpaceSim\BaseApplication.h"
#include "..\\SpaceSim\Camera.h"

Square::Square(Resource* resource, float radius) :
Super(resource),
    m_lowerleft(Vector2::zero() - Vector2(-radius, -radius)),
    m_upperright(Vector2::zero() + Vector2(radius, radius))
{
    m_boundingBox = Bbox(Vector3(m_lowerleft.x(), m_lowerleft.y(), 0.0f), Vector3(m_upperright.x(), m_upperright.y(), 0.0f));
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void Square::initialise(const ShaderInstance& shaderInstance)
{
    if (m_modelData.empty())
    {
        UNUSEDPARAM(shaderInstance);
        unsigned int bufferSize = 4 * (3 + 2) * sizeof(float);
        byte vertexData[4 * (3 + 2) * sizeof(float)]; //4 points with position and 1 2D textureCoordinates
        byte* data = vertexData;
        *(float*)data = m_lowerleft.x(); data += sizeof(float);
        *(float*)data = m_lowerleft.y(); data += sizeof(float);
        *(float*)data = 0.0f; data += sizeof(float);
        *(float*)data = 0.0f; data += sizeof(float);
        *(float*)data = 0.0f; data += sizeof(float);
        *(float*)data = m_upperright.x(); data += sizeof(float);
        *(float*)data = m_lowerleft.y(); data += sizeof(float);
        *(float*)data = 0.0f; data += sizeof(float);
        *(float*)data = 1.0f; data += sizeof(float);
        *(float*)data = 0.0f; data += sizeof(float);
        *(float*)data = m_upperright.x(); data += sizeof(float);
        *(float*)data = m_upperright.y(); data += sizeof(float);
        *(float*)data = 0.0f; data += sizeof(float);
        *(float*)data = 1.0f; data += sizeof(float);
        *(float*)data = 1.0f; data += sizeof(float);
        *(float*)data = m_lowerleft.x(); data += sizeof(float);
        *(float*)data = m_upperright.y(); data += sizeof(float);
        *(float*)data = 0.0f; data += sizeof(float);
        *(float*)data = 0.0f; data += sizeof(float);
        *(float*)data = 1.0f; data += sizeof(float);
        std::vector<unsigned int> texCoordDim;
        texCoordDim.push_back(2);

        VertexBuffer* vb = new VertexBuffer();
        const Technique* technique = m_modelData[0]->getShaderInstance().getMaterial().getEffect()->getTechnique("default");
        VertexDecalartionDesctriptor vertexDesc;
        vertexDesc.textureCoordinateDimensions = texCoordDim;
        vb->createBufferAndLayoutElements(getGameResource().getDeviceManager(), bufferSize, (void*)vertexData, false, vertexDesc, technique->getVertexShader()->getShaderBlob());
        m_modelData.push_back(new MeshGroup(vb, 0, shaderInstance));
        if (m_modelData[0]->getShaderInstance().getMaterial().getEffect() == nullptr)
        {
            m_modelData[0]->getShaderInstance().getMaterial().setEffect(getGameResource().getEffectCache().getEffect("simple_effect.fx"));
        }
        data = nullptr;
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

void Square::transform(Camera &cam)
{
    UNUSEDPARAM(cam);
}
