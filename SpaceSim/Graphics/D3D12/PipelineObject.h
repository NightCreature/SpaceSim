#pragma once

#include "D3D12.h"
#include "Graphics/Shaders.h"

struct VertexDeclarationDescriptor;

class PipelineObject
{
public:
    PipelineObject();
    ~PipelineObject();

    void CreatePipelineStateObject(ID3D12Device6* device);

    void SetRootSignature(ID3D12RootSignature* signature) { m_pipeLineStateDescriptor.pRootSignature = signature; }

    void SetVertextShader(const Shader& shader) 
    {
        m_pipeLineStateDescriptor.VS.BytecodeLength = shader.getShaderBlob()->GetBufferSize();
        m_pipeLineStateDescriptor.VS.pShaderBytecode = shader.getShaderBlob()->GetBufferPointer();
    }
    void SetPixelShader(const Shader& shader)
    {
        m_pipeLineStateDescriptor.PS.BytecodeLength = shader.getShaderBlob()->GetBufferSize();
        m_pipeLineStateDescriptor.PS.pShaderBytecode = shader.getShaderBlob()->GetBufferPointer();
    }
    void SetDomainShader(const Shader& shader)
    {
        m_pipeLineStateDescriptor.DS.BytecodeLength = shader.getShaderBlob()->GetBufferSize();
        m_pipeLineStateDescriptor.DS.pShaderBytecode = shader.getShaderBlob()->GetBufferPointer();
    }
    void SetHullShader(const Shader& shader)
    {
        m_pipeLineStateDescriptor.HS.BytecodeLength = shader.getShaderBlob()->GetBufferSize();
        m_pipeLineStateDescriptor.HS.pShaderBytecode = shader.getShaderBlob()->GetBufferPointer();
    }
    void SetGeometryShader(const Shader& shader)
    {
        m_pipeLineStateDescriptor.GS.BytecodeLength = shader.getShaderBlob()->GetBufferSize();
        m_pipeLineStateDescriptor.GS.pShaderBytecode = shader.getShaderBlob()->GetBufferPointer();
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
private:
    ID3D12PipelineState* m_pipelineObject = nullptr;
    //ID3D12RootSignature* m_rootSignature = nullptr;

    D3D12_GRAPHICS_PIPELINE_STATE_DESC m_pipeLineStateDescriptor = {0};
};