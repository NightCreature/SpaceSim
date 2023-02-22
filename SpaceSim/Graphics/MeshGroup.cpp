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
#include <Optick.h>

//shouldnt be here just want to be able to draw something
Matrix44 MeshGroup::m_projection;
Matrix44 MeshGroup::m_view;

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
MeshGroup::~MeshGroup()
{
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void MeshGroup::update( Resource* resource, RenderInstanceTree& renderInstance, float elapsedTime, const Matrix44& world, const Matrix44& view, const Matrix44& projection, const std::string& name, const Bbox& box )
{
//    PROFILE_EVENT("MeshGroup::update", Aqua);
//    //What if this just recorded its things into the CommandList?
//    //if (m_renderInstanceDirty || m_renderInstance == nullptr)
//    //{  
//        //if ( m_renderInstance != nullptr)
//        //{
//        //    delete m_renderInstance;
//        //    m_renderInstance = nullptr;
//        //}
//    if (m_renderInstance == nullptr)
//    {
//        PROFILE_EVENT("MeshGroup::update::Allocation", Black);
//        m_renderInstance = new RenderInstance(&m_geometryInstance, &m_shaderInstance);
//#ifdef _DEBUG
//        convertToWideString(name, m_renderInstance->m_name);
//        UNUSEDPARAM(name);
//#else
//        UNUSEDPARAM(name);
//#endif
//    }
//
//        WVPBufferContent wvpConstants; //= m_renderInstance->GetShaderInstance().getWVPConstants();
//        wvpConstants.m_projection = projection;
//        wvpConstants.m_view = view;
//        wvpConstants.m_world = m_world * world; 
//        auto vsConstants = m_renderInstance->GetShaderInstance().getVSConstantBufferSetup();
//        RenderResourceHelper resourceHelper(resource);
//        if (!vsConstants.empty())
//        {
//            resourceHelper.getResource().getDeviceManager().getDeferredDeviceContext()->UpdateSubresource(vsConstants[0], 0, 0, (void*)&wvpConstants, 0, 0); //Not sure about this
//        }
//
//        //Fix shader resource view references for the material.
//        m_renderInstance->GetShaderInstance().FixSrvReferences(resourceHelper.getWriteableResource());
//
//        auto psConstants = m_renderInstance->GetShaderInstance().getPSConstantBufferSetup();
//        if (!psConstants.empty())
//        {
//            resourceHelper.getResource().getDeviceManager().getDeferredDeviceContext()->UpdateSubresource(psConstants[0], 0, 0, (void*)&m_material.getMaterialCB(), 0, 0); //Not sure about this
//        }
//
//        UNUSEDPARAM(resource);
//        UNUSEDPARAM(world);
//        UNUSEDPARAM(view);
//        UNUSEDPARAM(projection);
//    //}
//    
//    if (m_renderInstance != nullptr)
//    {
//        m_renderInstance->setBoundingBox(box);
//        renderInstance.emplace_back(m_renderInstance);
//    }
//
//    UNUSEDPARAM(elapsedTime);

    UNUSEDPARAM(resource);
    UNUSEDPARAM(renderInstance);
    UNUSEDPARAM(elapsedTime);
    UNUSEDPARAM(world);
    UNUSEDPARAM(view);
    UNUSEDPARAM(projection);
    UNUSEDPARAM(name);
    UNUSEDPARAM(box);
}

void MeshGroup::Update(const Matrix44& world, const std::string& name, const Bbox& box)
{
    OPTICK_EVENT();
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
    WVPBufferContent wvpData;
    wvpData.m_world = world;
    wvpData.m_view = m_view;
    wvpData.m_projection = m_projection;

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
    OPTICK_EVENT();
    //PIXBeginEvent(list.m_list, 0, "Populate Commands: %s", m_name.c_str());

    //update constant buffers
    ShaderParameters& shaderParams = m_material.GetShaderParameters();

    auto& writableResource = RenderResourceHelper(resource).getWriteableResource();
    auto& effectCache = writableResource.getEffectCache();

    auto& heapManager = writableResource.getDescriptorHeapManager();

    //Setup textures
    ModelHelperFunctions::AssignTextureIndices(writableResource, m_material.getTextureHashes(), m_renderIndices);

    //Need to create the root descriptor tables we use here, copy in the descriptor handles from the heap to this area.

    //Might need to offset this but for now set the whole heap for CRV etc.
    ID3D12DescriptorHeap* ppHeaps[] = { heapManager.GetSRVCBVUAVHeap().m_heap, heapManager.GetSamplerHeap().m_heap };
    list.m_list->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

    //Set PSO object
    const Effect* effect = effectCache.getEffect(m_material.getEffectHash());
    const Technique* technique = effect->getTechnique(m_material.getTechnique());
    list.m_list->SetPipelineState(technique->GetPipelineState().m_pipelineObject);
    list.m_list->SetGraphicsRootSignature(technique->GetPipelineState().m_pipeLineStateDescriptor.pRootSignature);

    //Descriptor table might be an issue here, we need to set this through the texture objects themselves I think
    //We can set Shader Resource views and buffer resource views for constants
    ////This should come from teh material
    //size_t startOfRangeIndex = heapManager.GetSRVCBVUAVHeap().m_heap.GetCPUDescriptorHeapRangeRingBuffer(m_material.GetNumberOfNeededDescriptors());
    //size_t copiedNumberOfDescriptors = 0;
    //if (startOfRangeIndex == static_cast<size_t>(-1))
    //{
    //    //We have an error here and our range is out of scope
    //}

    //Count zero is always our mesh indices

    {
        OPTICK_EVENT("Set Root Params");
        //Currently param 0 is going to be the index buffer for all the streams
        list.m_list->SetGraphicsRoot32BitConstants(0, 32, &m_renderIndices, 0);
        //for (size_t counter = 1; counter < shaderParams.size(); ++counter)
        //{
        //    ShaderParameter& shaderParam = shaderParams[counter];
        //    {
        //        OPTICK_EVENT("Dispatch Root Param")
        //        auto index = shaderParam.m_data.index();
        //        switch (index)
        //        {
        //        case 0:
        //        case 1:
        //        {
        //            auto& constantBuffer = m_constantBuffers[shaderParam.m_rootParamIndex];
        //            list.m_list->SetGraphicsRootConstantBufferView(static_cast<UINT>(shaderParam.m_rootParamIndex), constantBuffer.second.GetConstantBuffer()->GetGPUVirtualAddress());
        //        }
        //        break;
        //        case 2:
        //        {
        //            list.m_list->SetGraphicsRootConstantBufferView(static_cast<UINT>(shaderParam.m_rootParamIndex), writableResource.getPerFrameDataStorage().GetConstantBuffer(0).GetConstantBuffer()->GetGPUVirtualAddress());
        //        }
        //        break;
        //        case 3:
        //        {
        //            OPTICK_EVENT("SetTexture PAram");
        //            auto* textureData = std::get_if<3>(&shaderParam.m_data);
        //            if (textureData != nullptr)
        //            {
        //                auto& textureHashes = m_material.getTextureHashes();
        //                TextureManager& tm = writableResource.getTextureManager();
        //                //need to allocate range here
        //                for (size_t textureSlotIndex = textureData->m_startingSlot; textureSlotIndex < textureData->m_numberOfTextures + textureData->m_startingSlot; ++textureSlotIndex)
        //                {
        //                    auto textureSlotIterator = std::find_if(textureHashes.begin(), textureHashes.end(), [textureSlotIndex](const auto& slotMapping) { return textureSlotIndex == slotMapping.m_textureSlot; });
        //                    if (textureSlotIterator != textureHashes.end())
        //                    {
        //                        //this should create a new descriptor range and set that in a table
        //                        const TextureInfo* texInfo = tm.getTexture(textureSlotIterator->m_textureHash);
        //                        if (texInfo != nullptr)
        //                        {
        //                            size_t heapIndex = texInfo->m_heapIndex;
        //                            auto textureCPUHandle = heapManager.GetSRVCBVUAVHeap().GetGPUDescriptorHandle(heapIndex);
        //                            list.m_list->SetGraphicsRootDescriptorTable(static_cast<UINT>(shaderParam.m_rootParamIndex), textureCPUHandle);
        //                        }
        //                        else
        //                        {
        //                            auto nullDescriptors = tm.GetNullDescriptor();
        //                            list.m_list->SetGraphicsRootDescriptorTable(static_cast<UINT>(shaderParam.m_rootParamIndex), heapManager.GetSRVCBVUAVHeap().GetGPUDescriptorHandle(nullDescriptors.second));
        //                        }
        //                    }
        //                }
        //            }
        //        }
        //        default:
        //            break;
        //        }
        //    }
        //}
    }

    {
        OPTICK_EVENT("Draw");
        OPTICK_GPU_EVENT("Draw MeshGroup");
        //Set Shader constants and samplers here this is different
        list.m_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //This needs to match the PSO setting think about particle effects
        //list.m_list->IASetPrimitiveTopology(static_cast<D3D_PRIMITIVE_TOPOLOGY>(m_primitiveLayout)); //Adjecency infocmtoin

        list.m_list->IASetIndexBuffer(&m_indexBuffer.GetBufferView());
        list.m_list->DrawIndexedInstanced(m_indexBuffer.getNumberOfIndecis(), 1, 0, 0, 0);
    }
    
    //PIXEndEvent(list.m_list);
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
size_t MeshGroup::CreateConstantBuffer(size_t size, size_t bufferNameHash, const DeviceManager& deviceManager, DescriptorHeap& heap)
{
    size_t heapIndex = InvalidDescriptorHeapIndex;
    if (size > 0 && !m_constantBuffers.contains(bufferNameHash))
    {
        ConstantBuffer cb;
        cb.Create(deviceManager, heap, size);
        m_constantBuffers[bufferNameHash] = cb;
        heapIndex = cb.GetHeapIndex();
    }
    else
    {
        MSG_ERROR_CHANNEL("MeshGroup", "Constant buffers already contains a definition for this constant buffer or the size is 0: %d size", size);
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
// @brief 
///-------------------------------------------------------------------------
MeshGroup::MeshGroup( const MeshGroup& source )
{
    m_world = source.m_world;
    m_renderInstanceDirty = true;
}
