
#include "Graphics/DebugBox.h"
#include "Graphics/EffectCache.h"
#include "Graphics/material.h"
#include "Math/matrix44.h"
#include "Graphics/Effect.h"
#include "Application/BaseApplication.h"

namespace DebugGraphics
{

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
DebugBox::DebugBox(Resource* resource, const Vector3& lowerLeft, const Vector3& upperRight) :
m_lowerLeft(lowerLeft),
m_upperRight(upperRight),
m_resource(resource)
{
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
DebugBox::~DebugBox()
{
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void DebugBox::initialise( const ShaderInstance& shaderInstance )
{
    UNUSEDPARAM(shaderInstance);
    box = new Model();
    GameResourceHelper helper(m_resource);
    if (box->getMeshData().empty())
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
        box->getMeshData().push_back(new MeshGroup(vb, ib, newShaderInstance));
        //if (m_modelData[0]->getShaderInstance().getMaterial().getEffect() == nullptr)
        //{
        box->getMeshData()[0]->getShaderInstance().getMaterial().setEffect(helper.getGameResource().getEffectCache().getEffect("debug_effect.xml"));
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


        const Technique* technique = box->getMeshData()[0]->getShaderInstance().getMaterial().getEffect()->getTechnique("default");
        VertexDeclarationDescriptor vertexDesc;
        vertexDesc.vertexColor = true;
        const VertexShader* shader = helper.getGameResource().getShaderCache().getVertexShader(technique->getVertexShader());
        vb->createBufferAndLayoutElements(helper.getGameResource().getDeviceManager(), numberOfBytes, (void*)boxVerts, false, vertexDesc, shader->getShaderBlob());
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

        ib->createBuffer(helper.getGameResource().getDeviceManager(), sizeof(indexData), (void*)&indexData[0], false, D3D11_BIND_INDEX_BUFFER);
        ib->setNumberOfIndecis( sizeof(indexData) / sizeof(unsigned int));
    }
    else
    {
        if (box->getMeshData()[0]->getShaderInstance().getMaterial().getEffect() == nullptr)
        {
            box->getMeshData()[0]->getShaderInstance().getMaterial().setEffect(helper.getGameResource().getEffectCache().getEffect("laser_effect.xml"));
        }
    }

    box->getMeshData()[0]->getGeometryInstance().setPrimitiveType((unsigned int)D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

}

