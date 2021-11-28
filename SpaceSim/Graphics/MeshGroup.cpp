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
    //Set Descriptor heaps but that might be in render system itself


    auto& writableResource = RenderResourceHelper(resource).getWriteableResource();
    auto& textureHeap = writableResource.getTextureManager().GetTextureDescriptorHeap();
    auto& samplerHeap = writableResource.getTextureManager().GetSamplerDescriptorHeap();
    auto& effectCache = writableResource.getEffectCache();

    auto& effectHeap = effectCache.GetDescriptorHeap();
    UNUSEDPARAM(effectHeap);

    ID3D12DescriptorHeap* ppHeaps[] = { textureHeap.m_heap, samplerHeap.m_heap };
    list.m_list->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
    //Set rootdiscriptor here

    //Descriptor table might be an issue here, we need to set this through the texture objects themselves I think
    //We can set Shader Resource views and buffer resource views for constants
    

    
    //Set PSO object
    auto& material = m_shaderInstance.getMaterial();
    Effect* effect = effectCache.getEffect(material.getEffectHash());
    Technique* technique = effect->getTechnique(material.getTechnique());

    //technique->m_psBuffer.UpdateCPUData(material.getMaterialCB());
    WVPBufferContent wvpData;
    wvpData.m_world = m_world;
    wvpData.m_view = RenderSystem::m_view;
    wvpData.m_projection = RenderSystem::m_projection;
    //technique->m_vsBuffer.UpdateCPUData(wvpData);

    //technique->GetPipelineState().BindToCommandList(list);
    list.m_list->SetPipelineState(technique->GetPipelineState().m_pipelineObject);
    list.m_list->SetGraphicsRootSignature(technique->GetPipelineState().m_pipeLineStateDescriptor.pRootSignature);

    //technique

    //list.m_list->SetGraphicsRootConstantBufferView(0, technique->m_vsBuffer.GetConstantBuffer()->GetGPUVirtualAddress()); //WVP
    //list.m_list->SetGraphicsRootConstantBufferView(1, technique->m_vsBuffer.GetConstantBuffer()->GetGPUVirtualAddress()); //Shadow
    //list.m_list->SetGraphicsRootConstantBufferView(2, technique->m_vsBuffer.GetConstantBuffer()->GetGPUVirtualAddress()); //Lights

    //list.m_list->SetGraphicsRootConstantBufferView(3, technique->m_psBuffer.GetConstantBuffer()->GetGPUVirtualAddress()); //material
    //list.m_list->SetGraphicsRootConstantBufferView(4, technique->m_psBuffer.GetConstantBuffer()->GetGPUVirtualAddress()); //lights again

    TextureManager& tm = writableResource.getTextureManager();

    size_t counter = 5;
    for (auto& textureSlot : material.getTextureHashes())
    {
        const TextureInfo* texInfo = tm.getTexture(textureSlot.m_textureHash);
        if (texInfo != nullptr)
        {
            //list.m_list->SetGraphicsRootDescriptorTable(counter, tm.GetTextureDescriptorHeap().GetGPUDescriptorHandle(texInfo->m_heapIndex));
            ++counter;
            //list.m_list->SetGraphicsRootShaderResourceView(textureSlot.m_textureSlot, );
        }
    }
	//commandList->SetGraphicsRootSignature(rootSignature.Get());
	//commandList->SetGraphicsRootDescriptorTable(RootParameterIndex::Texture1SRV, texture1);
	//commandList->SetGraphicsRootDescriptorTable(RootParameterIndex::Texture1Sampler, texture1Sampler);
	//commandList->SetGraphicsRootDescriptorTable(RootParameterIndex::Texture2SRV, texture2);
	//commandList->SetGraphicsRootDescriptorTable(RootParameterIndex::Texture2Sampler, texture2Sampler);


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



    //void FrameResource::PopulateCommandList(ID3D12GraphicsCommandList * pCommandList, ID3D12PipelineState * pPso1, ID3D12PipelineState * pPso2,
    //    UINT frameResourceIndex, UINT numIndices, D3D12_INDEX_BUFFER_VIEW * pIndexBufferViewDesc, D3D12_VERTEX_BUFFER_VIEW * pVertexBufferViewDesc,
    //    ID3D12DescriptorHeap * pCbvSrvDescriptorHeap, UINT cbvSrvDescriptorSize, ID3D12DescriptorHeap * pSamplerDescriptorHeap, ID3D12RootSignature * pRootSignature)
    //{
    //    // If the root signature matches the root signature of the caller, then
    //    // bindings are inherited, otherwise the bind space is reset.
    //    pCommandList->SetGraphicsRootSignature(pRootSignature);

    //    ID3D12DescriptorHeap* ppHeaps[] = { pCbvSrvDescriptorHeap, pSamplerDescriptorHeap };
    //    pCommandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
    //    pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //    pCommandList->IASetIndexBuffer(pIndexBufferViewDesc);
    //    pCommandList->IASetVertexBuffers(0, 1, pVertexBufferViewDesc);
    //    pCommandList->SetGraphicsRootDescriptorTable(0, pCbvSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
    //    pCommandList->SetGraphicsRootDescriptorTable(1, pSamplerDescriptorHeap->GetGPUDescriptorHandleForHeapStart());

    //    // Calculate the descriptor offset due to multiple frame resources.
    //    // 1 SRV + how many CBVs we have currently.
    //    UINT frameResourceDescriptorOffset = 1 + (frameResourceIndex * m_cityRowCount * m_cityColumnCount);
    //    CD3DX12_GPU_DESCRIPTOR_HANDLE cbvSrvHandle(pCbvSrvDescriptorHeap->GetGPUDescriptorHandleForHeapStart(), frameResourceDescriptorOffset, cbvSrvDescriptorSize);

    //    PIXBeginEvent(pCommandList, 0, L"Draw cities");
    //    BOOL usePso1 = TRUE;
    //    for (UINT i = 0; i < m_cityRowCount; i++)
    //    {
    //        for (UINT j = 0; j < m_cityColumnCount; j++)
    //        {
    //            // Alternate which PSO to use; the pixel shader is different on 
    //            // each just as a PSO setting demonstration.
    //            pCommandList->SetPipelineState(usePso1 ? pPso1 : pPso2);
    //            usePso1 = !usePso1;

    //            // Set this city's CBV table and move to the next descriptor.
    //            pCommandList->SetGraphicsRootDescriptorTable(2, cbvSrvHandle);
    //            cbvSrvHandle.Offset(cbvSrvDescriptorSize);

    //            pCommandList->DrawIndexedInstanced(numIndices, 1, 0, 0, 0);
    //        }
    //    }
    //    PIXEndEvent(pCommandList);
    //}
    auto depth = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
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
