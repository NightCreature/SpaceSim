#include "Graphics/OrientationAxis.h"

#include "Application/BaseApplication.h"
#include "Graphics/DeviceManager.h"
#include "Graphics/EffectCache.h"

#include "Core/tinyxml2.h"

#include <d3d11.h>
#include <assert.h>
#include "D3D12/CommandQueue.h"
#include "Model/MeshHelperFunctions.h"

std::vector<Vector3> m_vertices = 
{//		x		y	  z	
    {-1000.0f, 0.0f, 0.0f},//, 0xFFFFFFFF
    {1000.0f , 0.0f, 0.0f},//, 0xFFFFFFFF
    {0.0f, -1000.0f, 0.0f},//, 0xFFFFFFFF
    {0.0f, 1000.0f , 0.0f},//, 0xFFFFFFFF
    {0.0f, 0.0f, -1000.0f},//, 0xFFFFFFFF
    {0.0f, 0.0f, 1000.0f },//, 0xFFFFFFFF
    //Add lines for plus one on each axis
    {1.0f,  0.0f, 0.0f},
    {1.0f, 0.25f, 0.0f},// defines line indicating positive X orientation
    {0.0f,  1.0f, 0.0f},
    {0.25f, 1.0f, 0.0f},// defines line indicating indicates positive Y orientation
    {0.0f,  0.0f, 1.0f},
    {0.0f, 0.25f, 1.0f} // defines line indicating indicates positive Z orientation
};

std::vector<Color> m_colors =
{
    Color::red(),
    Color::red(),
    Color::green(),
    Color::green(),
    Color::blue(),
    Color::blue(),
    Color::red(),
    Color::red(),
    Color::green(),
    Color::green(),
    Color::blue(),
    Color::blue()
};

unsigned int indexBuffer[] =
{
    0,1,
    2,3,
    4,5,
    6,7,
    8,9,
    10,11
};

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void OrientationAxis::cleanup()
{
    m_vertexBuffer.Destroy();
    m_indexBuffer.cleanup();
    m_constantBuffer.Destroy();
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void OrientationAxis::initialise(Resource* resource, const DeviceManager& deviceManger, CommandList& list)
{
    RenderResourceHelper helper(resource);
    auto& srvCBVUAVHeap = helper.getWriteableResource().getDescriptorHeapManager().GetSRVCBVUAVHeap();

    m_effect = const_cast<Effect*>(helper.getWriteableResource().getEffectCache().getEffect("debug_color_shader.xml"_hash)); //Not the right name

    ////Move pointer to start of vertex array
    VertexDeclarationDescriptor vertexDesc;
    vertexDesc.vertexColor = true;
    VertexDataStreams dataStreams = CreateDataStreams(vertexDesc);
    dataStreams.m_streams[VertexStreamType::Position] = m_vertices;
    dataStreams.m_streams[VertexStreamType::Color] = m_colors;
    m_renderIndices = m_vertexBuffer.CreateBuffer(deviceManger, list, srvCBVUAVHeap, dataStreams);
    m_indexBuffer.Create(deviceManger, list, 12 * sizeof(unsigned int), &indexBuffer);
    m_renderIndices.transformIndex = static_cast<uint>(CreateConstantBuffer(sizeof(WVPData), deviceManger, srvCBVUAVHeap, "WorldTransform"));

    ModelHelperFunctions::AssignPerSceneIndices(helper.getWriteableResource(), m_renderIndices);
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void OrientationAxis::draw( const DeviceManager& deviceManager, const Matrix44& view, const Matrix44& projection, Resource* resource, CommandList& list)
{
    transform(deviceManager, view, projection);

    if (m_effect && m_effect->IsValid())
    {
        auto& writableResource = RenderResourceHelper(resource).getWriteableResource();
        auto& heapManager = writableResource.getDescriptorHeapManager();

        //Might need to offset this but for now set the whole heap for CRV etc.
        ID3D12DescriptorHeap* ppHeaps[] = { heapManager.GetSRVCBVUAVHeap().m_heap, heapManager.GetSamplerHeap().m_heap };
        list.m_list->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

        //Set PSO object

        const Technique* technique = m_effect->getTechnique("default"_hash);
        auto& pso = technique->GetPipelineState();
        if (pso.IsValid())
        {
            list.m_list->SetPipelineState(pso.m_pipelineObject);
            list.m_list->SetGraphicsRootSignature(pso.m_pipeLineStateDescriptor.pRootSignature);

            list.m_list->SetGraphicsRoot32BitConstants(0, 32, &m_renderIndices, 0);

            //Set Shader constants and samplers here this is different
            list.m_list->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST); //This needs to match the PSO setting think about particle effects
            //list.m_list->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(m_primitiveLayout)); //Adjecency infocmtoin

            list.m_list->IASetIndexBuffer(&m_indexBuffer.GetBufferView());
            list.m_list->DrawIndexedInstanced(m_indexBuffer.getNumberOfIndecis(), 1, 0, 0, 0);
        }
    }
}
///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void OrientationAxis::transform(const DeviceManager& deviceManager, const Matrix44& view, const Matrix44& projection)
{
    WVPData data;
    data.Projection = projection;
    data.View = view;
    Matrix44 world;
    world.identity();
    data.World = world;
    UNUSEDPARAM(deviceManager);

    m_constantBuffer.UpdateCpuData(data);
    m_constantBuffer.UpdateGpuData();
}

size_t OrientationAxis::CreateConstantBuffer(size_t size, const DeviceManager& deviceManager, DescriptorHeap& heap, std::string_view name)
{
    size_t heapIndex = InvalidDescriptorHeapIndex;

    m_constantBuffer.Create(deviceManager, heap, size, name);
    heapIndex = m_constantBuffer.GetHeapIndex();


    return heapIndex;
}