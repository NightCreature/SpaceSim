#pragma once

#include "D3D12.h"
#include "Graphics/Shaders.h"
#include "CommandQueue.h"

struct VertexDeclarationDescriptor;
struct IDxcBlob;

class PipelineObject
{
public:
    PipelineObject();
    ~PipelineObject();

    void CreatePipelineStateObject(ID3D12Device6* device);

    void SetRootSignature(ID3D12RootSignature* signature) { m_pipeLineStateDescriptor.pRootSignature = signature; }

    void SetVertextShader(const Shader& shader) 
    {
        m_pipeLineStateDescriptor.VS.BytecodeLength = shader.GetShaderBufferLength();
        m_pipeLineStateDescriptor.VS.pShaderBytecode = shader.GetShaderBufferPointer();
        m_rootSignatureBlob = shader.GetCompiledShader().m_rootSignatureBlob;
    }
    void SetPixelShader(const Shader& shader)
    {
        m_pipeLineStateDescriptor.PS.BytecodeLength = shader.GetShaderBufferLength();
        m_pipeLineStateDescriptor.PS.pShaderBytecode = shader.GetShaderBufferPointer();
    }
    void SetDomainShader(const Shader& shader)
    {
        m_pipeLineStateDescriptor.DS.BytecodeLength = shader.GetShaderBufferLength();
        m_pipeLineStateDescriptor.DS.pShaderBytecode = shader.GetShaderBufferPointer();
    }
    void SetHullShader(const Shader& shader)
    {
        m_pipeLineStateDescriptor.HS.BytecodeLength = shader.GetShaderBufferLength();
        m_pipeLineStateDescriptor.HS.pShaderBytecode = shader.GetShaderBufferPointer();
    }
    void SetGeometryShader(const Shader& shader)
    {
        m_pipeLineStateDescriptor.GS.BytecodeLength = shader.GetShaderBufferLength();
        m_pipeLineStateDescriptor.GS.pShaderBytecode = shader.GetShaderBufferPointer();
    }


    D3D12_BLEND_DESC& GetBLendDescriptor() { return m_pipeLineStateDescriptor.BlendState; }
    D3D12_DEPTH_STENCIL_DESC& GetDepthStencilDescriptor() { return m_pipeLineStateDescriptor.DepthStencilState; }
    D3D12_RASTERIZER_DESC& GetRasterizerDescriptor() { return m_pipeLineStateDescriptor.RasterizerState; }

    void SetVertexInformation( VertexDeclarationDescriptor& vertexDeclaration, D3D12_INDEX_BUFFER_STRIP_CUT_VALUE cutValue, D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveType);

    void SetRenderTargetInformation(size_t numRTs, DXGI_FORMAT* rtFormats, DXGI_FORMAT depthStencilFormat);
    void SetMultiSampling(unsigned int count, unsigned int quality)
    {
        m_pipeLineStateDescriptor.SampleDesc.Count = count;
        m_pipeLineStateDescriptor.SampleDesc.Quality = quality;

        m_pipeLineStateDescriptor.RasterizerState.MultisampleEnable = static_cast<BOOL>(count > 1);
    }

    void BindToCommandList(CommandList& commandList);

    bool IsValid() const { return (m_pipeLineStateDescriptor.VS.pShaderBytecode != nullptr && m_pipeLineStateDescriptor.PS.pShaderBytecode != nullptr) && m_pipelineObject != nullptr; }

    ID3D12PipelineState* m_pipelineObject = nullptr;
    D3D12_GRAPHICS_PIPELINE_STATE_DESC m_pipeLineStateDescriptor = { 0 };
    void Destroy();
private:
    
    //ID3D12RootSignature* m_rootSignature = nullptr;
    ID3DBlob* m_rootSignatureBlob = nullptr;

    
};