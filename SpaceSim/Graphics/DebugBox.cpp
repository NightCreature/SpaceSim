
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

const std::vector<Color> debugBoxColorStream =
{
    Color::cyan(),
    Color::cyan(),
    Color::cyan(),
    Color::cyan(),
    Color::cyan(),
    Color::cyan(),
    Color::cyan(),
    Color::cyan()
};

const unsigned int debugBoxIndexStream[] =
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
    //box = new Model();
    //RenderResourceHelper helper(m_resource);
    //if (box->getMeshData().empty())
    //{
    //    auto& group = box->CreateMeshGroup();

    //    std::vector<Vector3> positionStream = 
    //    {
    //        { m_lowerLeft.x(), m_lowerLeft.y(), m_lowerLeft.z() },
    //        { m_upperRight.x(), m_upperRight.y(), m_upperRight.z() },
    //        { m_upperRight.x(), m_lowerLeft.y(), m_lowerLeft.z() },
    //        { m_upperRight.x(), m_upperRight.y(), m_lowerLeft.z() },
    //        { m_lowerLeft.x(), m_upperRight.y(), m_lowerLeft.z() },
    //        { m_lowerLeft.x(), m_upperRight.y(), m_upperRight.z() },
    //        { m_lowerLeft.x(), m_lowerLeft.y(), m_upperRight.z() },
    //        { m_upperRight.x(), m_lowerLeft.y(), m_upperRight.z() }
    //    };

    //    //auto& commandQueue = helper.getWriteableResource().getCommandQueueManager().GetCommandQueue(helper.getResource().getResourceLoader().m_uploadQueueHandle);
    //    //auto& commandList = commandQueue.GetCommandList(helper.getResource().getResourceLoader().m_currentUploadCommandListHandle);
    //    //auto& srvCBVUAVHeap = helper.getWriteableResource().getDescriptorHeapManager().GetSRVCBVUAVHeap();

    //    VertexDataStreams vertexStreams;
    //    vertexStreams.m_streams[VertexStreamType::Position] = positionStream;
    //    vertexStreams.m_streams[VertexStreamType::Color] = debugBoxColorStream;

    //    group.GetVB().CreateBuffer(helper.getResource().getDeviceManager(), commandList, srvCBVUAVHeap, vertexStreams);

    //    group.GetIB().Create(helper.getResource().getDeviceManager(), commandList, sizeof(debugBoxIndexStream), (void*)&debugBoxIndexStream[0]);
    //    group.GetIB().setNumberOfIndecis( sizeof(debugBoxIndexStream) / sizeof(unsigned int));

    //    group.SetPrimitiveLayout(static_cast<uint32>(D3D_PRIMITIVE_TOPOLOGY_LINELIST));

    //    Material& mat = group.GetMaterial();
    //    //This late bind to the Pipeline object fucks up the stride we really need to make sure this is getting the same stride size as set in the PSO and also the topology
    //    mat.setEffectHash(hashString(getResourceNameFromFileName("Shaders\\Effects\\debug_effect.xml")));
    //    mat.Prepare(helper.getResource().getEffectCache());

    //    for (auto& shaderParam : mat.GetShaderParameters())
    //    {
    //        group.CreateConstantBuffer(GetVariantSize(shaderParam.m_data.index()), shaderParam.m_rootParamIndex, helper.getWriteableResource().getDeviceManager(), helper.getWriteableResource().getDescriptorHeapManager().GetSRVCBVUAVHeap());
    //    }
    //}
    //else
    //{
    //}
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

