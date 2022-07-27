#include "Graphics/Effect.h"
#include "Graphics/DeviceManager.h"
#include "Core/tinyxml2.h"

#include <assert.h>
#include <D3DCompiler.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "Graphics/DeviceManager.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "Graphics/D3DDebugHelperFunctions.h"
#include "Graphics/EffectCache.h"
#include "Core/Resource/RenderResource.h"
#include "Graphics/ShaderCache.h"
#include <stdio.h>
#include "VertexBuffer.h"
#include "D3D12/ShaderParamMatcher.h"





///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void Effect::setupEffect() const
{
    //ID3D11DeviceContext* deviceContext = deviceManager.getDeviceContext();
    //deviceContext->VSSetConstantBuffers(0, 1, &m_constantBuffers[0]);
    //deviceContext->PSSetConstantBuffers(0, 1, &m_constantBuffers[1]);
    //deviceContext->VSSetShader( m_vertexShader, NULL, 0 );
    //deviceContext->PSSetShader( m_pixelShader, NULL, 0 );
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
void Effect::deserialise(const tinyxml2::XMLElement* node, Resource* resource)
{
    for (const tinyxml2::XMLElement* childElement = node->FirstChildElement(); childElement != nullptr; childElement = childElement->NextSiblingElement())
    {
        auto elmentHash = hashString(childElement->Value());
        if (Technique::m_hash == elmentHash)
        {
            Technique technique(resource);
            technique.deserialise(childElement);
            m_techniques.emplace( std::pair<size_t, Technique>(technique.getNameHash(), technique) );
        }
    }
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
const Technique* Effect::getTechnique(const std::string& techniqueName) const
{
    return getTechnique(hashString(techniqueName));
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
const Technique* Effect::getTechnique(const size_t techniqueName) const
{
    auto it = m_techniques.find(techniqueName);
    if (it != end(m_techniques))
    {
        return &(it->second);
    }

    return nullptr;
}


///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
Technique* Effect::getTechnique(const std::string& techniqueName)
{
    return getTechnique(hashString(techniqueName));
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
Technique* Effect::getTechnique(const size_t techniqueName)
{
    auto it = m_techniques.find(techniqueName);
    if (it != end(m_techniques))
    {
        return &(it->second);
    }

    return nullptr;
}

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

    m_nameHash = hashString(techniqueName);
#ifdef _DEBUG
    m_name = techniqueName;
#endif // _DEBUG

    VertexDeclarationDescriptor vertextDeclaration;
    D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveTopology = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

    std::array<const Shader*, static_cast<std::underlying_type_t<ShaderType>>(ShaderType::Count)> loadedShaders = { nullptr };
    

    //Fix shader type
    for (const tinyxml2::XMLElement* childElement = element->FirstChildElement(); childElement != nullptr; childElement = childElement->NextSiblingElement())
    {
        auto elmentHash = hashString(childElement->Value());
        if (hashString("VertexShader") == elmentHash)
        {
            auto* shader = shaderCache.getVertexShader(shaderCache.getVertexShader(childElement, deviceManager));
            if (shader != nullptr)
            {
                m_pso.SetVertextShader(*shader);
                auto parameters = shader->GetParameters();
                std::copy(parameters.begin(), parameters.end(), std::back_inserter(m_shaderParameters));
            }

            loadedShaders[static_cast<std::underlying_type_t<ShaderType>>(ShaderType::eVertexShader)] = shader;
        }
        else if (hashString("HullShader") == elmentHash)
        {
            m_pso.SetHullShader(*shaderCache.getHullShader(shaderCache.getHullShader(childElement, deviceManager)));
            loadedShaders[static_cast<std::underlying_type_t<ShaderType>>(ShaderType::eHullShader)] = shaderCache.getGeometryShader(shaderCache.getGeometryShader(childElement, deviceManager));
        }
        else if (hashString("DomainShader") == elmentHash)
        {
            m_pso.SetDomainShader(*shaderCache.getDomainShader(shaderCache.getDomainShader(childElement, deviceManager)));
            loadedShaders[static_cast<std::underlying_type_t<ShaderType>>(ShaderType::eDomainShader)] = shaderCache.getGeometryShader(shaderCache.getGeometryShader(childElement, deviceManager));
        }
        else if (hashString("GeometryShader") == elmentHash)
        {
            m_pso.SetGeometryShader(*shaderCache.getGeometryShader(shaderCache.getGeometryShader(childElement, deviceManager)));
            loadedShaders[static_cast<std::underlying_type_t<ShaderType>>(ShaderType::eGeometryShader)] = shaderCache.getGeometryShader(shaderCache.getGeometryShader(childElement, deviceManager));
        }
        else if (hashString("PixelShader")  == elmentHash)
        {
            auto* shader = shaderCache.getPixelShader(shaderCache.getPixelShader(childElement, deviceManager));
            if (shader != nullptr)
            {
                m_pso.SetPixelShader(*shader);
            }
            loadedShaders[static_cast<std::underlying_type_t<ShaderType>>(ShaderType::ePixelShader)] = shader;
        }
        else if (hashString("ComputeShader") == elmentHash)
        {
            //This cant be set on a graphics pipeline state
            //m_pso.SetComputeShader(*shaderCache.getComputeShader(shaderCache.getComputeShader(childElement, deviceManager)));
            loadedShaders[static_cast<std::underlying_type_t<ShaderType>>(ShaderType::eComputeShader)] = nullptr;
        }
        else if (hashString("VertexDeclarationDescriptor") == elmentHash)
        {
            vertextDeclaration.Deserialise(childElement);
        }
        else if (hashString("BlendState") == elmentHash)
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
        else if (hashString("RasterState") == elmentHash)
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
        else if (hashString("DepthStencilState") == elmentHash)
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
        else if (hashString("PrimitiveType") == elmentHash)
        {
            attribute = childElement->FindAttribute("type");
            if (attribute != nullptr)
            {
                primitiveTopology = static_cast<D3D12_PRIMITIVE_TOPOLOGY_TYPE>(attribute->IntValue());
            }
            attribute = nullptr;
        }
    }

    //Build root paramter index layout 
    BuildRootParamaterLayout(loadedShaders);


    m_pso.SetVertexInformation(vertextDeclaration, D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED, primitiveTopology);
    m_pso.CreatePipelineStateObject(deviceManager.GetDevice());

    m_techniqueId = hashBinaryData(reinterpret_cast<char*>(&m_pso), sizeof(PipelineObject)); //Not sure this works well
    //m_techniqueId = ((size_t)(m_vertexShader + m_pixelShader) /*<< 32*/) | (m_hullShader + m_domainShader + m_geometryShader + m_computeShader);

    //ID3D11Device* device = deviceManager.getDevice();
    //D3D11_BUFFER_DESC wvpBufferDescriptor;
    //ZeroMemory(&wvpBufferDescriptor, sizeof(D3D11_BUFFER_DESC));
    //wvpBufferDescriptor.ByteWidth = sizeof(float)* 16 * 3;
    //wvpBufferDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    //if (FAILED(device->CreateBuffer(&wvpBufferDescriptor, 0, &m_constantBuffers[0])))
    //{
    //    MSG_TRACE_CHANNEL("EFFECT_ERROR", "failed to create the World, View and Porjection buffer for this effect");
    //        return;
    //}

    //D3DDebugHelperFunctions::SetDebugChildName(m_constantBuffers[0], FormatString("Constant Buffer 0 Technique %s has: vs: %ull, ps: %ull", techniqueName.c_str(), m_vertexShader, m_pixelShader));

    //D3D11_BUFFER_DESC materialDescriptor;
    //ZeroMemory(&materialDescriptor, sizeof(D3D11_BUFFER_DESC));
    //materialDescriptor.ByteWidth = sizeof(float)* 20;
    //materialDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    //HRESULT hr = device->CreateBuffer(&materialDescriptor, 0, &m_constantBuffers[1]);
    //if (FAILED(hr))
    //{
    //    MSG_TRACE_CHANNEL("EFFECT_ERROR", "failed to create the material buffer for this effect: 0x%x(%s)", hr, D3DDebugHelperFunctions::D3DErrorCodeToString(hr));
    //        return;
    //}
    //D3DDebugHelperFunctions::SetDebugChildName(m_constantBuffers[1], FormatString("Constant Buffer 1 Technique %s has: vs: %ull, ps: %ull", techniqueName.c_str(), m_vertexShader, m_pixelShader));
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void Technique::BuildRootParamaterLayout(const std::array<const Shader*, static_cast<std::underlying_type_t<ShaderType>>(ShaderType::Count)>& shaders)
{
    RootParamtersInfo rootParameterInfo;
    for (size_t counter = 0; counter < shaders.size(); ++counter)
    {
        if (shaders[counter] != nullptr)
        {
            ShaderParamMatcher matcher(shaders[counter]->getShaderBlob());
            matcher.MatchSignatureWithRefeclection(rootParameterInfo);
            if (static_cast<ShaderType>(counter) == ShaderType::eVertexShader)
            {
                rootParameterInfo = matcher.GetRootParametersInfo();
            }

            auto& shaderParams = matcher.GetBoundParameters();
            m_shaderParameters.insert(m_shaderParameters.end(), shaderParams.begin(), shaderParams.end());
        }
    }

//#ifdef _DEBUG
//    MSG_TRACE_CHANNEL("Technique", "Trace Shader Parameters for Technique: %s", m_name.c_str());
//#endif
//    MSG_TRACE_CHANNEL("Technique", "Trace Shader Parameters for Technique: %d", m_nameHash);
//    PrintParameters(m_shaderParameters);
    //ShaderParamMatcher matcher(m_shaderBlob);
    //if (matcher.MatchSignatureWithRefeclection())
    //{
    //    rootParameterInfo = matcher.GetRootParametersInfo();
    //}
    //m_parameters = matcher.GetBoundParameters();
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void Technique::setWVPContent(const DeviceManager& deviceManager, const WVPBufferContent& wvpContent) const
{
    //deviceManager.getDeviceContext()->UpdateSubresource(m_constantBuffers[0], 0, 0, (void*)&wvpContent, 0, 0);
    UNUSEDPARAM(deviceManager);
    UNUSEDPARAM(wvpContent);
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void Technique::setMaterialContent(const DeviceManager& deviceManager, const MaterialContent& materialContent) const
{
    //deviceManager.getDeviceContext()->UpdateSubresource(m_constantBuffers[1], 0, 0, (void*)&materialContent, 0, 0);
    UNUSEDPARAM(deviceManager);
    UNUSEDPARAM(materialContent);
}