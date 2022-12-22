
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
    delete box;
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void DebugBox::initialise()
{
    box = new Model();
    RenderResourceHelper helper(m_resource);
    if (box->getMeshData().empty())
    {
        auto& group = box->CreateMeshGroup();

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

        group.GetVB().Create(helper.getResource().getDeviceManager(), commandList, numberOfBytes, (void*)boxVerts, vertexDesc.GetVertexStride());
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

        group.GetIB().Create(helper.getResource().getDeviceManager(), commandList, sizeof(indexData), (void*)&indexData[0]);
        group.GetIB().setNumberOfIndecis( sizeof(indexData) / sizeof(unsigned int));

        group.SetPrimitiveLayout(static_cast<uint32>(D3D_PRIMITIVE_TOPOLOGY_LINELIST));

        Material& mat = group.GetMaterial();
        //This late bind to the Pipeline object fucks up the stride we really need to make sure this is getting the same stride size as set in the PSO and also the topology
        mat.setEffectHash(hashString(getResourceNameFromFileName("Shaders\\Effects\\debug_effect.xml")));
        mat.Prepare(helper.getResource().getEffectCache());

        for (auto& shaderParam : mat.GetShaderParameters())
        {
            group.CreateConstantBuffer(GetVariantSize(shaderParam.m_data.index()), shaderParam.m_rootParamIndex, helper.getWriteableResource().getDeviceManager(), helper.getWriteableResource().getDescriptorHeapManager().GetSRVCBVUAVHeap());
        }
    }
    else
    {
    }
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void DebugBox::UpdateCbs()
{
    box->UpdateCbs();
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void DebugBox::PopulateCommandlist(Resource* resource, CommandList& commandList)
{
    box->PopulateCommandlist(resource, commandList);
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void DebugBox::Update(const MessageSystem::RenderInformation::RenderInfo& context)
{
    box->Update(context);
}

}

