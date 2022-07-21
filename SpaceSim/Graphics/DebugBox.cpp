
#include "Graphics/DebugBox.h"
#include "Graphics/EffectCache.h"
#include "Graphics/material.h"
#include "Math/matrix44.h"
#include "Graphics/Effect.h"
#include "Application/BaseApplication.h"

#include "Core/Resource/RenderResource.h"
#include "Graphics/EffectCache.h"
#include "D3D12/CommandQueue.h"

namespace DebugGraphics
{

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
DebugBox::DebugBox(Resource* resource, const Vector3& lowerLeft, const Vector3& upperRight) :
m_lowerLeft(lowerLeft),
m_upperRight(upperRight),
m_resource(resource)
{
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
DebugBox::~DebugBox()
{
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void DebugBox::initialise( const ShaderInstance& shaderInstance, const Matrix44& view, const Matrix44& projection)
{
    UNUSEDPARAM(shaderInstance);
    box = new Model();
    RenderResourceHelper helper(m_resource);
    if (box->getMeshData().empty())
    {
        VertexBuffer* vb = new VertexBuffer();
        IndexBuffer* ib = new IndexBuffer();

        WVPBufferContent wvp;
        Matrix44 temp;
        temp.identity();
        wvp.m_world = temp;
        wvp.m_view = view;
        wvp.m_projection = projection;
        Material mat;
        mat.setEffectHash(hashString("debug_effect.xml"));
        box->getMeshData().push_back(new MeshGroup(vb, ib, mat, helper.getWriteableResource().getDeviceManager()));
        //if (m_modelData[0]->getShaderInstance().getMaterial().getEffect() == nullptr)
        //{
        //box->getMeshData()[0]->getShaderInstance().getMaterial().setEffect(helper.getResource().getEffectCache().getEffect("debug_effect.xml"));
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

        VertexDeclarationDescriptor vertexDesc;
        vertexDesc.vertexColor = true;
        

        auto& commandQueue = helper.getWriteableResource().getCommandQueueManager().GetCommandQueue(helper.getResource().getResourceLoader().m_uploadQueueHandle);
        auto& commandList = commandQueue.GetCommandList(helper.getResource().getResourceLoader().m_currentUploadCommandListHandle);

        vb->Create(helper.getResource().getDeviceManager(), commandList, numberOfBytes, (void*)boxVerts, vertexDesc.GetVertexStride());
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

        ib->Create(helper.getResource().getDeviceManager(), commandList, sizeof(indexData), (void*)&indexData[0]);
        ib->setNumberOfIndecis( sizeof(indexData) / sizeof(unsigned int));
    }
    else
    {
    }

    box->getMeshData()[0]->getGeometryInstance().setPrimitiveType((unsigned int)D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

}

