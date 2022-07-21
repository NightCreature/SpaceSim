#include "Graphics/MeshGroup.h"
#include "Application/BaseApplication.h"
#include "Graphics/DeviceManager.h"
#include "Core/Resource/GameResource.h"
#include "Graphics/RenderInstance.h"
#include "Gameplay/bbox.h"
#ifdef _DEBUG
#include "Core/StringOperations/StringHelperFunctions.h"
#endif
#include "Core/Profiler/ProfilerMacros.h"

#include "Core/Resource/RenderResource.h"
#include "Core/Types/TypeHelpers.h"
#include "D3D12/DescriptorHeapManager.h"
#include <variant>

//shouldnt be here just want to be able to draw something
Matrix44 MeshGroup::m_projection;
Matrix44 MeshGroup::m_view;

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
MeshGroup::~MeshGroup()
{
    if (m_renderInstance != nullptr)
    {
        delete m_renderInstance;
        m_renderInstance = nullptr;
    }
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void MeshGroup::update( Resource* resource, RenderInstanceTree& renderInstance, float elapsedTime, const Matrix44& world, const Matrix44& view, const Matrix44& projection, const std::string& name, const Bbox& box )
{
    PROFILE_EVENT("MeshGroup::update", Aqua);
    //What if this just recorded its things into the CommandList?
    //if (m_renderInstanceDirty || m_renderInstance == nullptr)
    //{  
        //if ( m_renderInstance != nullptr)
        //{
        //    delete m_renderInstance;
        //    m_renderInstance = nullptr;
        //}
		if (m_renderInstance == nullptr)
		{
            PROFILE_EVENT("MeshGroup::update::Allocation", Black);
			m_renderInstance = new RenderInstance(&m_geometryInstance, &m_shaderInstance);
#ifdef _DEBUG
			convertToWideString(name, m_renderInstance->m_name); 
            UNUSEDPARAM(name);
#else
			UNUSEDPARAM(name);
#endif
		}

        WVPBufferContent wvpConstants; //= m_renderInstance->GetShaderInstance().getWVPConstants();
        wvpConstants.m_projection = projection;
        wvpConstants.m_view = view;
        wvpConstants.m_world = m_world * world; 
        auto vsConstants = m_renderInstance->GetShaderInstance().getVSConstantBufferSetup();
        RenderResourceHelper resourceHelper(resource);
        if (!vsConstants.empty())
        {
            resourceHelper.getResource().getDeviceManager().getDeferredDeviceContext()->UpdateSubresource(vsConstants[0], 0, 0, (void*)&wvpConstants, 0, 0); //Not sure about this
        }

        //Fix shader resource view references for the material.
        m_renderInstance->GetShaderInstance().FixSrvReferences(resourceHelper.getWriteableResource());

        auto psConstants = m_renderInstance->GetShaderInstance().getPSConstantBufferSetup();
        if (!psConstants.empty())
        {
            resourceHelper.getResource().getDeviceManager().getDeferredDeviceContext()->UpdateSubresource(psConstants[0], 0, 0, (void*)&m_material.getMaterialCB(), 0, 0); //Not sure about this
        }

        UNUSEDPARAM(resource);
        UNUSEDPARAM(world);
        UNUSEDPARAM(view);
        UNUSEDPARAM(projection);
    //}
    
    if (m_renderInstance != nullptr)
    {
        m_renderInstance->setBoundingBox(box);
        renderInstance.emplace_back(m_renderInstance);
    }

    UNUSEDPARAM(elapsedTime);
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark This needs to be rewritten were this is called because we should be updating the CBV values here and create a populate commandlist function were we actually do the commands currently in this function
///-----------------------------------------------------------------------------
void MeshGroup::Update(Resource* resource, CommandList& list, float elapsedTime, const Matrix44& world, const std::string& name, const Bbox& box)
{
    //update constant buffers
    ShaderParameters& shaderParams = m_material.GetShaderParameters();

    auto& writableResource = RenderResourceHelper(resource).getWriteableResource();
    auto& effectCache = writableResource.getEffectCache();

    auto& heapManager = writableResource.getDescriptorHeapManager();

    auto& shaderVisibleHeap = heapManager.GetShaderVisibleHeap();
    //Need to create the root descriptor tables we use here, copy in the descriptor handles from the heap to this area.

    //Might need to offset this but for now set the whole heap for CRV etc.
    ID3D12DescriptorHeap* ppHeaps[] = { heapManager.GetSRVCBVUAVHeap().m_heap, heapManager.GetSamplerHeap().m_heap };
    list.m_list->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
    //Set rootdiscriptor here

    //Descriptor table might be an issue here, we need to set this through the texture objects themselves I think
    //We can set Shader Resource views and buffer resource views for constants
    

    
    //Set PSO object
    const Effect* effect = effectCache.getEffect(m_material.getEffectHash());
    const Technique* technique = effect->getTechnique(m_material.getTechnique());

    //This should come from teh material
    size_t startOfRangeIndex = shaderVisibleHeap.GetCPUDescriptorHeapRangeRingBuffer(m_material.GetNumberOfNeededDescriptors());
    size_t copiedNumberOfDescriptors = 0;
    if (startOfRangeIndex == static_cast<size_t>(-1))
    {
        //We have an error here and our range is out of scope
    }
    //auto device = writableResource.getDeviceManager().GetDevice();

    //technique->m_psBuffer.UpdateCPUData(material.getMaterialCB());
    //WVPBufferContent wvpData;
    //wvpData.m_world = m_world;
    //wvpData.m_view = RenderSystem::m_view;
    //wvpData.m_projection = RenderSystem::m_projection;
    //technique->m_vsBuffer.UpdateCPUData(wvpData);

    //technique->GetPipelineState().BindToCommandList(list);
    list.m_list->SetPipelineState(technique->GetPipelineState().m_pipelineObject);
    list.m_list->SetGraphicsRootSignature(technique->GetPipelineState().m_pipeLineStateDescriptor.pRootSignature);

    //technique
    //MSG_TRACE_CHANNEL("Mesh Group", "Drawing mesh with following deatils:");
    //MSG_TRACE_CHANNEL("Mesh Group", "\tEffect Hash: %s", effect->m_name.c_str());
    //MSG_TRACE_CHANNEL("Mesh Group", "\tTechnique Hash: %d", technique->getTechniqueId());
    //MSG_TRACE_CHANNEL("Mesh Group", "\tNumber of Root Params: %d", shaderParams.size());
    
    for (size_t counter = 0; counter < shaderParams.size(); ++counter)
    {
        ShaderParameter& shaderParam = shaderParams[counter];
        auto index = shaderParam.m_data.index();
        switch (index)
        {
        case 0:
        {
            auto* constantBuffer = std::get_if<0>(&shaderParam.m_data);


            //if (shaderParam.m_rootParamIndex == 0 || shaderParam.m_rootParamIndex == 2)
            {
                //This should be world stuffs
                WVPBufferContent wvpBuffer;
                wvpBuffer.m_projection = m_projection;
                wvpBuffer.m_view = m_view;
                wvpBuffer.m_world = world;
                constantBuffer->UpdateCPUData(wvpBuffer);
                constantBuffer->UpdateGPUData();
            }

            list.m_list->SetGraphicsRootConstantBufferView(static_cast<UINT>(shaderParam.m_rootParamIndex),constantBuffer->GetConstantBuffer()->GetGPUVirtualAddress());
            //MSG_TRACE_CHANNEL("Mesh Group", "Set WVP data for param: %d", shaderParam.m_rootParamIndex);
        }
        break;
        case 1:
        {
            auto* constantBuffer = std::get_if<1>(&shaderParam.m_data);
            constantBuffer->UpdateCPUData(m_material.getMaterialCB());
            constantBuffer->UpdateGPUData();
            list.m_list->SetGraphicsRootConstantBufferView(static_cast<UINT>(shaderParam.m_rootParamIndex), constantBuffer->GetConstantBuffer()->GetGPUVirtualAddress());
            //MSG_TRACE_CHANNEL("Mesh Group", "Set material data for param: %d", shaderParam.m_rootParamIndex);
        }
        break;
        case 2:
        {
            auto* constantBuffer = std::get_if<2>(&shaderParam.m_data);
            constantBuffer->UpdateGPUData();
            list.m_list->SetGraphicsRootConstantBufferView(static_cast<UINT>(shaderParam.m_rootParamIndex), constantBuffer->GetConstantBuffer()->GetGPUVirtualAddress());
            //MSG_TRACE_CHANNEL("Mesh Group", "Set per frame data for param: %d", shaderParam.m_rootParamIndex);
        }
        break;
        case 3:
        //{
        //    TextureManager& tm = writableResource.getTextureManager();
        //    auto nullDescriptors = tm.GetNullDescriptor();
        //    list.m_list->SetGraphicsRootDescriptorTable(static_cast<UINT>(shaderParam.m_rootParamIndex), heapManager.GetSRVCBVUAVHeap().GetGPUDescriptorHandle(nullDescriptors.second));
        //    //MSG_TRACE_CHANNEL("Mesh Group", "Set texture data for param: %d", shaderParam.m_rootParamIndex);
        //}
        {
            auto* textureData = std::get_if<3>(&shaderParam.m_data);
            if (textureData != nullptr)
            {
                auto& textureHashes = m_material.getTextureHashes();
                TextureManager& tm = writableResource.getTextureManager();
                //need to allocate range here
                for (size_t textureSlotIndex = textureData->m_startingSlot; textureSlotIndex < textureData->m_numberOfTextures + textureData->m_startingSlot; ++textureSlotIndex)
                {
                    auto textureSlotIterator = std::find_if(textureHashes.begin(), textureHashes.end(), [textureSlotIndex](const auto& slotMapping) { return textureSlotIndex == slotMapping.m_textureSlot; });
                    if (textureSlotIterator != textureHashes.end())
                    {
                        //this should create a new descriptor range and set that in a table
                        const TextureInfo* texInfo = tm.getTexture(textureSlotIterator->m_textureHash);
                        if (texInfo != nullptr)
                        {
                            //auto& actualDescriptor = shaderVisibleHeap.GetCPUDescriptorHandle(copiedNumberOfDescriptors);
                            //device->CopyDescriptorsSimple(1, actualDescriptor, textureDescriptorHeap.GetCPUDescriptorHandle(texInfo->m_heapIndex), D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
                            //list.m_list->SetGraphicsRootDescriptorTable(counter, tm.GetTextureDescriptorHeap().GetGPUDescriptorHandle(texInfo->m_heapIndex));
                            ++counter;
                            ++copiedNumberOfDescriptors;

                            // We need to copy the descriptors here so we can set a table
                            //size_t descriptorStartIndex = heapManager.GetSRVCBVUAVHeap().GetCPUDescriptorHeapRange(textureData->m_numberOfTextures);
                            //auto& cpuDescriptorHandle = heapManager.GetSRVCBVUAVHeap().GetCPUDescriptorHandle(descriptorStartIndex);
                            size_t heapIndex = texInfo->m_heapIndex;
                            auto textureCPUHandle = heapManager.GetSRVCBVUAVHeap().GetGPUDescriptorHandle(heapIndex);
                            //device->CopyDescriptorsSimple(1, cpuDescriptorHandle, textureCPUHandle, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV); //This fails because the descriptor heap is set to shader visible
                            //You should allocate CPU and GPU descriptor handles at the same time, so basically our heap index can be used for GPU and CPU creation of handles
                            //D3D12_GPU_DESCRIPTOR_HANDLE descriptor;
                            list.m_list->SetGraphicsRootDescriptorTable(static_cast<UINT>(shaderParam.m_rootParamIndex), textureCPUHandle);
                        }
                        else
                        {
                            //Create a proper null descriptor range here potentially so we always have something bound 
                            // // E:\SDK\DirectX-Graphics-Samples\Samples\Desktop\D3D12xGPU\src\bin\x64\Debug check out sample its doing more work than most others
                            //Set a null descriptor
                            auto nullDescriptors = tm.GetNullDescriptor();
                            list.m_list->SetGraphicsRootDescriptorTable(static_cast<UINT>(shaderParam.m_rootParamIndex), heapManager.GetSRVCBVUAVHeap().GetGPUDescriptorHandle(nullDescriptors.second));
                        }
                    }
                }
            }
        }
        default:
            break;
        }
    }


    //Set Shader constants and samplers here this is different
    list.m_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //Adjecency infocmtoin
    D3D12_VERTEX_BUFFER_VIEW vertexBuffers[] = { m_geometryInstance.getVB()->GetBufferView() };
    list.m_list->IASetVertexBuffers(0, 1, vertexBuffers);
    const D3D12_INDEX_BUFFER_VIEW& indexBuffer = m_geometryInstance.getIB()->GetBufferView();
    list.m_list->IASetIndexBuffer(&indexBuffer);

    list.m_list->DrawIndexedInstanced(m_geometryInstance.getIB()->getNumberOfIndecis(), 1, 0, 0, 0);
    
    UNUSEDPARAM(box);
    UNUSEDPARAM(name);
    UNUSEDPARAM(world);
    UNUSEDPARAM(elapsedTime);
    UNUSEDPARAM(resource);
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
MeshGroup::MeshGroup( const MeshGroup& source )
{
    m_world = source.m_world;
    m_geometryInstance = source.getGeometryInstance();
    m_shaderInstance = source.getShaderInstance();
    m_renderInstance = nullptr;
    m_renderInstanceDirty = true;
}
