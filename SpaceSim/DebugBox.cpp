#include "DebugBox.h"
#include "EffectCache.h"
#include "material.h"
#include "matrix44.h"
#include "Effect.h"
#include "BaseApplication.h"

namespace DebugGraphics
{

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
DebugBox::DebugBox(Resource* resource, const Vector3& lowerLeft, const Vector3& upperRight) :
Super(resource),
m_lowerLeft(lowerLeft),
m_upperRight(upperRight)
{
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
DebugBox::~DebugBox()
{
}

#pragma optimize ( "", off)
//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void DebugBox::initialise( const ShaderInstance& shaderInstance )
{
    UNUSEDPARAM(shaderInstance);
    if (m_modelData.empty())
    {
        VertexBuffer* vb = new VertexBuffer();
        IndexBuffer* ib = new IndexBuffer();

        WVPBufferContent wvp;
        Matrix44 temp;
        temp.identity();
        wvp.m_world = temp;
        wvp.m_view = Application::m_view;
        wvp.m_projection = Application::m_projection;
        ShaderInstance newShaderInstance(wvp, Material());
        m_modelData.push_back(new MeshGroup(vb, ib, newShaderInstance));
        //if (m_modelData[0]->getShaderInstance().getMaterial().getEffect() == nullptr)
        //{
        m_modelData[0]->getShaderInstance().getMaterial().setEffect(getGameResource().getEffectCache().getEffect("debug_effect.xml"));
        //}

        ColorVertex boxVerts[] = 
        {
            { m_lowerLeft.x(), m_lowerLeft.y(), m_lowerLeft.z(), 0.0f, 1.0f, 1.0f, 1.0f},
            { m_upperRight.x(), m_upperRight.y(), m_upperRight.z(), 0.0f, 1.0f, 1.0f, 1.0f},
            { m_upperRight.x(), m_lowerLeft.y(), m_lowerLeft.z(), 0.0f, 1.0f, 1.0f, 1.0f},
            { m_upperRight.x(), m_upperRight.y(), m_lowerLeft.z(), 0.0f, 1.0f, 1.0f, 1.0f},
            { m_lowerLeft.x(), m_upperRight.y(), m_lowerLeft.z(), 0.0f, 1.0f, 1.0f, 1.0f},
            { m_lowerLeft.x(), m_upperRight.y(), m_upperRight.z(), 0.0f, 1.0f, 1.0f, 1.0f},
            { m_lowerLeft.x(), m_lowerLeft.y(), m_upperRight.z(), 0.0f, 1.0f, 1.0f, 1.0f},
            { m_upperRight.x(), m_lowerLeft.y(), m_upperRight.z(), 0.0f, 1.0f, 1.0f, 1.0f}
        };

        unsigned int numberOfBytes = sizeof(boxVerts);


        const Technique* technique = m_modelData[0]->getShaderInstance().getMaterial().getEffect()->getTechnique("default");
        VertexDecalartionDesctriptor vertexDesc;
        vertexDesc.vertexColor = true;
        const VertexShader* shader = getGameResource().getShaderCache().getVertexShader(technique->getVertexShader());
        vb->createBufferAndLayoutElements(getGameResource().getDeviceManager(), numberOfBytes, (void*)boxVerts, false, vertexDesc, shader->getShaderBlob());
        unsigned int indexData[] =
        {
            0, 2,
            2, 3,
            3, 4,
            4, 0,
            1, 5,
            5, 6,
            6, 7,
            7, 1,
            0, 6,
            2, 7,
            4, 5,
            3, 1
        };

        ib->createBuffer(getGameResource().getDeviceManager(), sizeof(indexData), (void*)&indexData[0], false, D3D11_BIND_INDEX_BUFFER);
        ib->setNumberOfIndecis( sizeof(indexData) / sizeof(unsigned int));
    }
    else
    {
        if (m_modelData[0]->getShaderInstance().getMaterial().getEffect() == nullptr)
        {
            m_modelData[0]->getShaderInstance().getMaterial().setEffect(getGameResource().getEffectCache().getEffect("laser_effect.xml"));
        }
    }

    m_modelData[0]->getGeometryInstance().setPrimitiveType((unsigned int)D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

}
#pragma optimize ( "", on)
