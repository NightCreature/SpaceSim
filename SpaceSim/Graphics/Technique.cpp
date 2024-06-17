#include "Graphics/Technique.h"

#include "Core/Resource/RenderResource.h"
#include "Core/StringOperations/HashString.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "Graphics/D3D12/DeviceManagerD3D12.h"
#include "Graphics/ShaderCache.h"
#include "Graphics/VertexBuffer.h"

#include <d3d12.h>
#include "imgui.h"



///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void Technique::deserialise(const tinyxml2::XMLElement* element)
{
    RenderResourceHelper renderResource = { m_resource };
    ShaderCache& shaderCache = renderResource.getWriteableResource().getShaderCache();
    const DeviceManager& deviceManager = renderResource.getResource().getDeviceManager();

    std::string techniqueName = "default_technique";
    const tinyxml2::XMLAttribute* attribute = element->FindAttribute("name");
    if (attribute != nullptr)
    {
        techniqueName = attribute->Value();
    }

    m_nameHash = HashString(techniqueName);
#ifdef _DEBUG
    m_name = techniqueName;
#endif // _DEBUG

    VertexDeclarationDescriptor vertextDeclaration;
    D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveTopology = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    //So this is reading and kicking compile commands for shaders, which is asynchrounous, so we can't fill out the PSO here.
    for (const tinyxml2::XMLElement* childElement = element->FirstChildElement(); childElement != nullptr; childElement = childElement->NextSiblingElement())
    {
        auto elmentHash = Hashing::hashString(childElement->Value());
        if (HashString("VertexShader") == elmentHash)
        {
            shaderCache.getVertexShader(childElement, deviceManager, [&pso = m_pso](Shader& shader) { pso.SetVertextShader(shader); });
        }
        else if (HashString("HullShader") == elmentHash)
        {
            shaderCache.getHullShader(childElement, deviceManager, [&pso = m_pso](Shader& shader) { pso.SetHullShader(shader);  });
        }
        else if (HashString("DomainShader") == elmentHash)
        {
            shaderCache.getDomainShader(childElement, deviceManager, [&pso = m_pso](Shader& shader) { pso.SetDomainShader(shader); });
        }
        else if (HashString("GeometryShader") == elmentHash)
        {
            shaderCache.getGeometryShader(childElement, deviceManager, [&pso = m_pso](Shader& shader) { pso.SetGeometryShader(shader); });
        }
        else if (HashString("PixelShader") == elmentHash)
        {
            shaderCache.getPixelShader(childElement, deviceManager, [&pso = m_pso](Shader& shader) { pso.SetPixelShader(shader); });
        }
        else if (HashString("ComputeShader") == elmentHash)
        {
            shaderCache.getComputeShader(childElement, deviceManager, [](Shader& shader) { MSG_TRACE_CHANNEL("Technique", "Trying to create a compute shader %s", shader.getFileName().c_str()); });
            //This cant be set on a graphics pipeline state
            //m_pso.SetComputeShader(*shaderCache.getComputeShader(shaderCache.getComputeShader(childElement, deviceManager)));
        }
        else if (HashString("VertexDeclarationDescriptor") == elmentHash)
        {
            vertextDeclaration.Deserialise(childElement);
        }
        else if (HashString("BlendState") == elmentHash)
        {
            //    <BlendState  alphablend = "false" / >
            auto& blendDesc = m_pso.GetBLendDescriptor();
            attribute = childElement->FindAttribute("alphablend");
            if (attribute != nullptr)
            {
                blendDesc.RenderTarget[0].BlendEnable = attribute->IntValue();
                if (blendDesc.RenderTarget[0].BlendEnable)
                {

                    blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
                    blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
                    blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
                    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_SRC_ALPHA;
                    blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
                    blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
                }
            }
            attribute = nullptr;

        }
        else if (HashString("RasterState") == elmentHash)
        {
            //    <RasterState Cullmode = "back" FillMode = "solid" FrontCCW = "true" / >
            auto& rasterDesc = m_pso.GetRasterizerDescriptor();
            attribute = childElement->FindAttribute("Cullmode");
            if (attribute != nullptr)
            {
                rasterDesc.CullMode = static_cast<D3D12_CULL_MODE>(attribute->IntValue());
            }
            attribute = nullptr;
            attribute = childElement->FindAttribute("Fillmode");
            if (attribute != nullptr)
            {
                rasterDesc.FillMode = static_cast<D3D12_FILL_MODE>(attribute->IntValue());
            }
            attribute = nullptr;
            attribute = childElement->FindAttribute("FrontCCW");
            if (attribute != nullptr)
            {
                rasterDesc.FrontCounterClockwise = attribute->IntValue();
            }
            attribute = nullptr;
        }
        else if (HashString("DepthStencilState") == elmentHash)
        {
            //    <DepthStencilState depth_enabled = "true" stencil_enabled = "false" / >
            auto& dsDesc = m_pso.GetDepthStencilDescriptor();
            attribute = childElement->FindAttribute("depth_enabled");
            if (attribute != nullptr)
            {
                dsDesc.DepthEnable = attribute->IntValue();
            }
            attribute = nullptr;
            attribute = childElement->FindAttribute("stencil_enabled");
            if (attribute != nullptr)
            {
                dsDesc.StencilEnable = attribute->IntValue();
            }
            attribute = nullptr;
        }
        else if (HashString("PrimitiveType") == elmentHash)
        {
            attribute = childElement->FindAttribute("type");
            if (attribute != nullptr)
            {
                primitiveTopology = static_cast<D3D12_PRIMITIVE_TOPOLOGY_TYPE>(attribute->IntValue());
            }
            attribute = nullptr;
        }
    }

    //Build root paramter index layout no longer needed with heap indexing
    //BuildRootParamaterLayout(loadedShaders);


    m_pso.SetVertexInformation(vertextDeclaration, D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED, primitiveTopology);
    //m_pso.CreatePipelineStateObject(deviceManager.GetDevice());

    m_techniqueId = HashString(Hashing::hashBinaryData(reinterpret_cast<char*>(&m_pso), sizeof(PipelineObject))); //Not sure this works well
    //m_techniqueId = ((size_t)(m_vertexShader + m_pixelShader) /*<< 32*/) | (m_hullShader + m_domainShader + m_geometryShader + m_computeShader);
}

void Technique::OnDebugImgui() const
{
#ifdef _DEBUG
    m_pso.OnDebugImgui();
#endif
}
