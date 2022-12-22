#include "Graphics/D3D12/PipelineObject.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "Graphics/VertexBuffer.h"
#include "D3D12X.h"


///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
PipelineObject::PipelineObject()
{
    ZeroMemory(&m_pipeLineStateDescriptor, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

    //Default state values should deserialise this but this ease of use
    DXGI_FORMAT rtFormats[] = { DXGI_FORMAT_R8G8B8A8_UNORM };
    SetRenderTargetInformation(1, rtFormats, DXGI_FORMAT_D32_FLOAT);
    
    D3D12_RASTERIZER_DESC& rasterizerStateDesc = m_pipeLineStateDescriptor.RasterizerState;
    rasterizerStateDesc.CullMode = D3D12_CULL_MODE_BACK;
    rasterizerStateDesc.FillMode = D3D12_FILL_MODE_SOLID;
    rasterizerStateDesc.AntialiasedLineEnable = false;
    rasterizerStateDesc.DepthBias = 0;
    rasterizerStateDesc.DepthBiasClamp = 0.0f;
    rasterizerStateDesc.FrontCounterClockwise = true;
    rasterizerStateDesc.MultisampleEnable = false;
    rasterizerStateDesc.SlopeScaledDepthBias = 0.0f;
    rasterizerStateDesc.DepthClipEnable = false;


    D3D12_DEPTH_STENCIL_DESC& depthStencilStateDesc = m_pipeLineStateDescriptor.DepthStencilState;
    depthStencilStateDesc.DepthEnable = true;
    depthStencilStateDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    depthStencilStateDesc.DepthFunc = D3D12_COMPARISON_FUNC_GREATER_EQUAL; //THis should be greater or maybe less then when we go to flipped Z again
    depthStencilStateDesc.StencilEnable = 0;
    depthStencilStateDesc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
    depthStencilStateDesc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
    depthStencilStateDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
    depthStencilStateDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
    depthStencilStateDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
    depthStencilStateDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
    depthStencilStateDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
    depthStencilStateDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
    depthStencilStateDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
    depthStencilStateDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;

    D3D12_BLEND_DESC& blendStateDesc = m_pipeLineStateDescriptor.BlendState;
    blendStateDesc.AlphaToCoverageEnable = false;
    blendStateDesc.IndependentBlendEnable = false;
    for (unsigned int counter = 0; counter < 8; ++counter)
    {
        blendStateDesc.RenderTarget[counter].BlendEnable = false;
        blendStateDesc.RenderTarget[counter].SrcBlend = D3D12_BLEND_ONE;
        blendStateDesc.RenderTarget[counter].DestBlend = D3D12_BLEND_ZERO;
        blendStateDesc.RenderTarget[counter].BlendOp = D3D12_BLEND_OP_ADD;
        blendStateDesc.RenderTarget[counter].SrcBlendAlpha = D3D12_BLEND_ONE;
        blendStateDesc.RenderTarget[counter].DestBlendAlpha = D3D12_BLEND_ZERO;
        blendStateDesc.RenderTarget[counter].BlendOpAlpha = D3D12_BLEND_OP_ADD;
        blendStateDesc.RenderTarget[counter].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
    }

    m_pipeLineStateDescriptor.SampleMask = 0xFFFFFFFF;

    //Must match backbuffer if only rendering to that
    SetMultiSampling(1, 0);

    /*D3D11_BLEND_DESC blendDescriptor;
    blendDescriptor.AlphaToCoverageEnable = FALSE;
    blendDescriptor.IndependentBlendEnable = FALSE;
    for (unsigned int counter = 0; counter < 8; ++counter)
    {
        blendDescriptor.RenderTarget[0].BlendEnable = TRUE;
        blendDescriptor.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
        blendDescriptor.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
        blendDescriptor.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
        blendDescriptor.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
        blendDescriptor.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
        blendDescriptor.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
        blendDescriptor.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
    }*/

}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
PipelineObject::~PipelineObject()
{
    //if (m_pipelineObject != nullptr)
    //{
    //    m_pipelineObject->Release();
    //    m_pipelineObject = nullptr;
    //}
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void PipelineObject::CreatePipelineStateObject(ID3D12Device6* device)
{
    if (m_pipeLineStateDescriptor.VS.pShaderBytecode == nullptr)
    {
        MSG_WARN_CHANNEL("Pipelineobject", "Vertex Shader is null, did the shader compile. Can't create pipeline object");
        return;
    }

    HRESULT hr  = device->CreateRootSignature(0, m_pipeLineStateDescriptor.VS.pShaderBytecode, m_pipeLineStateDescriptor.VS.BytecodeLength, IID_PPV_ARGS(&m_pipeLineStateDescriptor.pRootSignature));
    if (hr != S_OK)
    {
        MSG_TRACE_CHANNEL("PipelineObject", "Failed to create Root signature from shader data with error: 0x%x, %s", hr, getLastErrorMessage(hr));
        assert(false);
    }

    hr = device->CreateGraphicsPipelineState(&m_pipeLineStateDescriptor, IID_PPV_ARGS(&m_pipelineObject));

    if (hr != S_OK)
    {
        MSG_TRACE_CHANNEL("PipelineObject", "Failed to create Pipeline state with error: 0x%x, %s", hr, getLastErrorMessage(hr));
    }

    //CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
    //rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void PipelineObject::SetVertexInformation(VertexDeclarationDescriptor& vertexDeclaration, D3D12_INDEX_BUFFER_STRIP_CUT_VALUE cutValue, D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveType)
{
    size_t stride = 0;
    const auto& vertexElements = vertexDeclaration.createInputElementLayout(stride);
    if (vertexElements.size() > 0)
    {
        m_pipeLineStateDescriptor.InputLayout = { &vertexElements[0], static_cast<unsigned int> (vertexElements.size()) };
    }
    else
    {
        m_pipeLineStateDescriptor.InputLayout.NumElements = 0;
        m_pipeLineStateDescriptor.InputLayout.pInputElementDescs = nullptr;
    }
    m_pipeLineStateDescriptor.IBStripCutValue = cutValue;
    m_pipeLineStateDescriptor.PrimitiveTopologyType = primitiveType; // type of primitive we are rendering, point, line, triangle, patch
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void PipelineObject::SetRenderTargetInformation(size_t numRTs, DXGI_FORMAT* rtFormats, DXGI_FORMAT depthStencilFormat)
{
    m_pipeLineStateDescriptor.NumRenderTargets = static_cast<unsigned int>(numRTs);
    for (size_t counter = 0; counter < numRTs; ++counter)
    {
        m_pipeLineStateDescriptor.RTVFormats[counter] = rtFormats[counter];
    }
    m_pipeLineStateDescriptor.DSVFormat = depthStencilFormat;

}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void PipelineObject::BindToCommandList(CommandList& commandList)
{
    commandList.m_list->SetPipelineState(m_pipelineObject);
}

