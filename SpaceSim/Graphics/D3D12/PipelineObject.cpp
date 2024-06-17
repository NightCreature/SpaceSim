#include "Graphics/D3D12/PipelineObject.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "Graphics/VertexBuffer.h"
#include "D3D12X.h"
#include <dxcapi.h>
#include "imgui.h"

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
    rasterizerStateDesc.CullMode = D3D12_CULL_MODE_NONE;
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

    HRESULT hr  = device->CreateRootSignature(0, m_rootSignatureBlob->GetBufferPointer(), m_rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&m_pipeLineStateDescriptor.pRootSignature));
    if (hr != S_OK)
    {
        MSG_TRACE_CHANNEL("PipelineObject", "Failed to create Root signature from shader data with error: 0x%x, %s", hr, getLastErrorMessage(hr));
        assert(false);
    }
    else
    {
        MSG_TRACE_CHANNEL("PipelineObject", "Created Root signature from shader data");
    }

    hr = device->CreateGraphicsPipelineState(&m_pipeLineStateDescriptor, IID_PPV_ARGS(&m_pipelineObject));

    if (hr != S_OK)
    {
        MSG_TRACE_CHANNEL("PipelineObject", "Failed to create Pipeline state with error: 0x%x, %s", hr, getLastErrorMessage(hr));
    }
    else
    {
        MSG_TRACE_CHANNEL("PipelineObject", "Created PSO");
    }

    MSG_TRACE_CHANNEL_FMT("pipelinobject", "{} PS shader", m_pipeLineStateDescriptor.PS.pShaderBytecode != nullptr ? "Has a" : "Doesn't have a");

    //CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
    //rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void PipelineObject::SetVertexInformation(VertexDeclarationDescriptor& vertexDeclaration, D3D12_INDEX_BUFFER_STRIP_CUT_VALUE cutValue, D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveType)
{
    UNUSEDPARAM(vertexDeclaration);

    //size_t stride = 0;
    //const auto& vertexElements;// = vertexDeclaration.createInputElementLayout(stride);
    //if (vertexElements.size() > 0)
    //{
    //    m_pipeLineStateDescriptor.InputLayout = { &vertexElements[0], static_cast<unsigned int> (vertexElements.size()) };
    //}
    //else
    //{
        m_pipeLineStateDescriptor.InputLayout.NumElements = 0;
        m_pipeLineStateDescriptor.InputLayout.pInputElementDescs = nullptr;
   //}
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

void PipelineObject::Destroy()
{
    if (m_pipelineObject)
    {
        m_pipelineObject->Release();
    }
    if (m_rootSignatureBlob)
    {
        m_rootSignatureBlob->Release();
    }
}

void PipelineObject::OnDebugImgui() const
{
    ImGui::Text("PipelineObject");
    ImGui::Text("Root Signature: %p", m_pipeLineStateDescriptor.pRootSignature);
    ImGui::Text("PipelineObject: %p", m_pipelineObject);
    if (ImGui::TreeNode("Shader State"))
    {
        if (m_pipeLineStateDescriptor.VS.pShaderBytecode != nullptr)
        {
            ImGui::Text("Vertex Shader: %p", m_pipeLineStateDescriptor.VS.pShaderBytecode);
        }
        if (m_pipeLineStateDescriptor.PS.pShaderBytecode != nullptr)
        {
            ImGui::Text("Pixel Shader: %p", m_pipeLineStateDescriptor.PS.pShaderBytecode);
        }
        if (m_pipeLineStateDescriptor.DS.pShaderBytecode != nullptr)
        {
            ImGui::Text("Domain Shader: %p", m_pipeLineStateDescriptor.DS.pShaderBytecode);
        }
        if (m_pipeLineStateDescriptor.HS.pShaderBytecode != nullptr)
        {
            ImGui::Text("Hull Shader: %p", m_pipeLineStateDescriptor.HS.pShaderBytecode);
        }
        if (m_pipeLineStateDescriptor.GS.pShaderBytecode != nullptr)
        {
            ImGui::Text("Geometry Shader: %p", m_pipeLineStateDescriptor.GS.pShaderBytecode);
        }

        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Input State"))
    {
        ImGui::Text("Input Layout: %d", m_pipeLineStateDescriptor.InputLayout.NumElements);
        ImGui::Text("IB Strip Cut Value: %d", m_pipeLineStateDescriptor.IBStripCutValue);
        ImGui::Text("Primitive Topology Type: %d", m_pipeLineStateDescriptor.PrimitiveTopologyType);

        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Depth Stencil State"))
    {
        ImGui::Text("Depth Enable: %s", m_pipeLineStateDescriptor.DepthStencilState.DepthEnable ? "true" : "false");
        ImGui::Text("Depth Write Mask: %d", m_pipeLineStateDescriptor.DepthStencilState.DepthWriteMask);
        ImGui::Text("Depth Func: %d", m_pipeLineStateDescriptor.DepthStencilState.DepthFunc);
        ImGui::Text("Stencil Enable: %s", m_pipeLineStateDescriptor.DepthStencilState.StencilEnable ? "true" : "false");
        ImGui::Text("Stencil Read Mask: %d", m_pipeLineStateDescriptor.DepthStencilState.StencilReadMask);
        ImGui::Text("Stencil Write Mask: %d", m_pipeLineStateDescriptor.DepthStencilState.StencilWriteMask);
        ImGui::Text("Front Face Stencil Func: %d", m_pipeLineStateDescriptor.DepthStencilState.FrontFace.StencilFunc);
        ImGui::Text("Front Face Stencil Fail Op: %d", m_pipeLineStateDescriptor.DepthStencilState.FrontFace.StencilFailOp);
        ImGui::Text("Front Face Stencil Depth Fail Op: %d", m_pipeLineStateDescriptor.DepthStencilState.FrontFace.StencilDepthFailOp);
        ImGui::Text("Front Face Stencil Pass Op: %d", m_pipeLineStateDescriptor.DepthStencilState.FrontFace.StencilPassOp);
        ImGui::Text("Back Face Stencil Func: %d", m_pipeLineStateDescriptor.DepthStencilState.BackFace.StencilFunc);
        ImGui::Text("Back Face Stencil Fail Op: %d", m_pipeLineStateDescriptor.DepthStencilState.BackFace.StencilFailOp);
        ImGui::Text("Back Face Stencil Depth Fail Op: %d", m_pipeLineStateDescriptor.DepthStencilState.BackFace.StencilDepthFailOp);
        ImGui::Text("Back Face Stencil Pass Op: %d", m_pipeLineStateDescriptor.DepthStencilState.BackFace.StencilPassOp);
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Render Target and Blend State"))
    {
        ImGui::Text("RTV Formats: %d", m_pipeLineStateDescriptor.NumRenderTargets);
        ImGui::Text("DSV Format: %d", m_pipeLineStateDescriptor.DSVFormat);
        
        ImGui::Text("Blend State: %s", m_pipeLineStateDescriptor.BlendState.AlphaToCoverageEnable ? "true" : "false");
        ImGui::Text("Blend State: %s", m_pipeLineStateDescriptor.BlendState.IndependentBlendEnable ? "true" : "false");
        for (unsigned int counter = 0; counter < 8; ++counter)
        {
            if (ImGui::TreeNode(fmt::format("RenderTarget {}", counter).c_str()))
            {
                ImGui::Text("Blend Enable: %s", m_pipeLineStateDescriptor.BlendState.RenderTarget[counter].BlendEnable ? "true" : "false");
                ImGui::Text("Src Blend: %d", m_pipeLineStateDescriptor.BlendState.RenderTarget[counter].SrcBlend);
                ImGui::Text("Dest Blend: %d", m_pipeLineStateDescriptor.BlendState.RenderTarget[counter].DestBlend);
                ImGui::Text("Blend Op: %d", m_pipeLineStateDescriptor.BlendState.RenderTarget[counter].BlendOp);
                ImGui::Text("Src Blend Alpha: %d", m_pipeLineStateDescriptor.BlendState.RenderTarget[counter].SrcBlendAlpha);
                ImGui::Text("Dest Blend Alpha: %d", m_pipeLineStateDescriptor.BlendState.RenderTarget[counter].DestBlendAlpha);
                ImGui::Text("Blend Op Alpha: %d", m_pipeLineStateDescriptor.BlendState.RenderTarget[counter].BlendOpAlpha);
                ImGui::Text("RenderTargetWriteMask: %d", m_pipeLineStateDescriptor.BlendState.RenderTarget[counter].RenderTargetWriteMask);

                ImGui::TreePop();
            }
        }

        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Sample State"))
    {
        ImGui::Text("Sample Mask: %d", m_pipeLineStateDescriptor.SampleMask);
        ImGui::Text("Sample Desc: %d", m_pipeLineStateDescriptor.SampleDesc.Count);
        ImGui::Text("Sample Desc: %d", m_pipeLineStateDescriptor.SampleDesc.Quality);
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Rasterizer State"))
    {
        ImGui::Text("Cull Mode: %d", m_pipeLineStateDescriptor.RasterizerState.CullMode);
        ImGui::Text("Fill Mode: %d", m_pipeLineStateDescriptor.RasterizerState.FillMode);
        ImGui::Text("Antialised Line Enable: %s", m_pipeLineStateDescriptor.RasterizerState.AntialiasedLineEnable ? "true" : "false");
        ImGui::Text("Depth Bias: %d", m_pipeLineStateDescriptor.RasterizerState.DepthBias);
        ImGui::Text("Depth Bias Clamp: %d", m_pipeLineStateDescriptor.RasterizerState.DepthBiasClamp);
        ImGui::Text("Front Counter Clockwise: %s", m_pipeLineStateDescriptor.RasterizerState.FrontCounterClockwise ? "true" : "false");
        ImGui::Text("Multisample Enable: %s", m_pipeLineStateDescriptor.RasterizerState.MultisampleEnable ? "true" : "false");
        ImGui::Text("Slope Scaled Depth Bias: %d", m_pipeLineStateDescriptor.RasterizerState.SlopeScaledDepthBias);
        ImGui::Text("Depth Clip Enable: %s", m_pipeLineStateDescriptor.RasterizerState.DepthClipEnable ? "true" : "false");

        ImGui::TreePop();
    }



}

