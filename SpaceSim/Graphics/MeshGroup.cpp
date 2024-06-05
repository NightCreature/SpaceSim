#include "Graphics/MeshGroup.h"
#include "Application/BaseApplication.h"
#include "Graphics/DeviceManager.h"
#include "Core/Resource/GameResource.h"
#include "Graphics/RenderInstance.h"
#include "Graphics/Model/MeshHelperFunctions.h"
#include "Gameplay/bbox.h"
#ifdef _DEBUG
#include "Core/StringOperations/StringHelperFunctions.h"
#endif
#include "Core/Profiler/ProfilerMacros.h"

#include "Core/Resource/RenderResource.h"
#include "Core/Types/TypeHelpers.h"
#include "D3D12/DescriptorHeapManager.h"
#include <variant>

#include <pix3.h>

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
MeshGroup::~MeshGroup()
{
}

void MeshGroup::Update(const Matrix44& world, const std::string& name, const Bbox& box)
{
    PROFILE_FUNCTION();
    //PIXBeginEvent(list.m_list, 0, "Update Mesh Group: %s", m_name.c_str());

    //ShaderParameters& shaderParams = m_material.GetShaderParameters();

    //for (size_t counter = 0; counter < shaderParams.size(); ++counter)
    //{
    //    ShaderParameter& shaderParam = shaderParams[counter];
    //    auto index = shaderParam.m_data.index();
    //    switch (index)
    //    {
    //    case 0:
    //    {
    //        OPTICK_EVENT("Update WVP Constants");
    //        auto constantData = *(std::get_if<0>(&shaderParam.m_data));
    //        auto& constantBuffer = m_constantBuffers[shaderParam.m_rootParamIndex];
    //        //This should be world stuffs
    //        constantData.m_projection = m_projection;
    //        constantData.m_view = m_view;
    //        constantData.m_world = world;
    //        constantBuffer.second.UpdateCpuData(constantData);
    //        constantBuffer.second.UpdateGpuData();
    //    }
    //    break;
    //    case 1:
    //    {
    //        auto& constantBuffer = m_constantBuffers[shaderParam.m_rootParamIndex];
    //        constantBuffer.second.UpdateCpuData(m_material.getMaterialCB());
    //        constantBuffer.second.UpdateGpuData();
    //    }
    //    break;
    //    case 2:
    //    {
    //    }
    //    break;
    //    case 3: //This could create the descriptor tables if we have more
    //    default:
    //        break;
    //    }
    //}

    //Update Content buffer data
    WVPData wvpData;
    wvpData.World = world;

    ConstantBuffer & wvpBuffer = m_constantBuffers["WVPBuffer"_hash];
    wvpBuffer.UpdateCpuData(wvpData);
    wvpBuffer.UpdateGpuData();

    ConstantBuffer& materialData = m_constantBuffers["Material"_hash];
    materialData.UpdateCpuData(m_material.getMaterialCB());
    materialData.UpdateGpuData();
    //PIXEndEvent(list.m_list);


    UNUSEDPARAM(box);
    UNUSEDPARAM(name);
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void MeshGroup::Update(const MessageSystem::RenderInformation::RenderInfo& context)
{
    Update(context.m_world, context.m_name, Bbox());

    //Update indices here
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark This needs to be rewritten were this is called because we should be updating the CBV values here and create a populate commandlist function were we actually do the commands currently in this function
///-----------------------------------------------------------------------------
void MeshGroup::PopulateCommandlist(Resource* resource, CommandList& list)
{
    PROFILE_FUNCTION();
    //PIXBeginEvent(list.m_list, 0, "Populate Commands: %s", m_name.c_str());

    //update constant buffers
    auto& writableResource = RenderResourceHelper(resource).getWriteableResource();
    auto& effectCache = writableResource.getEffectCache();

    auto& heapManager = writableResource.getDescriptorHeapManager();

    //Setup textures, this should probably move to the prepare of the model and when textures are loaded
    ModelHelperFunctions::AssignTextureIndices(writableResource, m_material.getTextureHashes(), m_renderIndices);

    //Need to create the root descriptor tables we use here, copy in the descriptor handles from the heap to this area.

    //Might need to offset this but for now set the whole heap for CRV etc.
    ID3D12DescriptorHeap* ppHeaps[] = { heapManager.GetSRVCBVUAVHeap().m_heap, heapManager.GetSamplerHeap().m_heap };
    list.m_list->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

    //Set PSO object
    const Effect* effect = effectCache.getEffect(m_material.getEffectHash());
    if (effect != nullptr)
    {
        const Technique* technique = effect->getTechnique(m_material.getTechnique());
        if (technique != nullptr && technique->GetPipelineState().IsValid())
        {
            list.m_list->SetPipelineState(technique->GetPipelineState().m_pipelineObject);
            list.m_list->SetGraphicsRootSignature(technique->GetPipelineState().m_pipeLineStateDescriptor.pRootSignature);

            {
                PROFILE_TAG("Set Root Params");
                //Currently param 0 is going to be the index buffer for all the streams
                list.m_list->SetGraphicsRoot32BitConstants(0, 32, &m_renderIndices, 0);
            }

            {
                PROFILE_TAG("Draw");
                OPTICK_GPU_EVENT("Draw MeshGroup");
                //Set Shader constants and samplers here this is different
                list.m_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //This needs to match the PSO setting think about particle effects
                //list.m_list->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(m_primitiveLayout)); //Adjecency infocmtoin

                list.m_list->IASetIndexBuffer(&m_indexBuffer.GetBufferView());
                list.m_list->DrawIndexedInstanced(m_indexBuffer.getNumberOfIndecis(), 1, 0, 0, 0);
            }

            //PIXEndEvent(list.m_list);
        }
    }
    else
    {
        PROFILE_TAG("Failing Drawcall");
    }
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
size_t MeshGroup::CreateConstantBuffer(size_t size, size_t bufferNameHash, const DeviceManager& deviceManager, DescriptorHeap& heap, const std::string_view& name)
{
    size_t heapIndex = InvalidDescriptorHeapIndex;
    if (size > 0 && !m_constantBuffers.contains(bufferNameHash))
    {
        ConstantBuffer cb;
        cb.Create(deviceManager, heap, size, name);
        m_constantBuffers[bufferNameHash] = cb;
        heapIndex = cb.GetHeapIndex();
    }
    else
    {
        MSG_ERROR_CHANNEL("MeshGroup", "Constant buffers already contains a definition for this constant buffer or the size is 0: %d size, %s", size, name.data());
    }

    return heapIndex;
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void MeshGroup::Cleanup()
{
    m_vertexBuffer.Destroy();
    m_indexBuffer.cleanup();
    for (auto& constantBufferPair : m_constantBuffers)
    {
        constantBufferPair.second.Destroy();
    }
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void MeshGroup::UpdateRenderIndices()
{

}

///-------------------------------------------------------------------------
// @brief There is an error here copying the index buffer kills us
///-------------------------------------------------------------------------
MeshGroup::MeshGroup( const MeshGroup& source )
{
    m_world = source.m_world;
    m_renderInstanceDirty = true;
    
    m_renderIndices = source.m_renderIndices;
    m_material = source.m_material;
    //m_indexBuffer = source.m_indexBuffer;

    m_name = source.m_name;

    m_primitiveLayout = source.m_primitiveLayout;
}
