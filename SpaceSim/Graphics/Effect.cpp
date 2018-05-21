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
#include "Core/Resource/RenderResource.h"
#include "Graphics/ShaderCache.h"
#include <stdio.h>





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

    for (const tinyxml2::XMLElement* childElement = element->FirstChildElement(); childElement != nullptr; childElement = childElement->NextSiblingElement())
    {
        auto elmentHash = hashString(childElement->Value());
        if (VertexShader::m_hash == elmentHash)
        {
            m_vertexShader = shaderCache.getVertexShader(childElement, deviceManager);
        }
        else if (HullShader::m_hash == elmentHash)
        {
            m_hullShader = shaderCache.getHullShader(childElement, deviceManager);
        }
        else if (DomainShader::m_hash == elmentHash)
        {
            m_domainShader = shaderCache.getDomainShader(childElement, deviceManager);
        }
        else if (GeometryShader::m_hash == elmentHash)
        {
            m_geometryShader = shaderCache.getGeometryShader(childElement, deviceManager);
        }
        else if (PixelShader::m_hash == elmentHash)
        {
            m_pixelShader = shaderCache.getPixelShader(childElement, deviceManager);
        }
        else if (ComputeShader::m_hash == elmentHash)
        {
            m_computeShader = shaderCache.getComputeShader(childElement, deviceManager);
        }
    }

    m_techniqueId = ((size_t)(m_vertexShader + m_pixelShader) /*<< 32*/) | (m_hullShader + m_domainShader + m_geometryShader + m_computeShader);

    ID3D11Device* device = deviceManager.getDevice();
    D3D11_BUFFER_DESC wvpBufferDescriptor;
    ZeroMemory(&wvpBufferDescriptor, sizeof(D3D11_BUFFER_DESC));
    wvpBufferDescriptor.ByteWidth = sizeof(float)* 16 * 3;
    wvpBufferDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    if (FAILED(device->CreateBuffer(&wvpBufferDescriptor, 0, &m_constantBuffers[0])))
    {
        MSG_TRACE_CHANNEL("EFFECT_ERROR", "failed to create the World, View and Porjection buffer for this effect");
            return;
    }

    D3DDebugHelperFunctions::SetDebugChildName(m_constantBuffers[0], FormatString("Constant Buffer 0 Technique %s has: vs: %ull, ps: %ull", techniqueName.c_str(), m_vertexShader, m_pixelShader));

    D3D11_BUFFER_DESC materialDescriptor;
    ZeroMemory(&materialDescriptor, sizeof(D3D11_BUFFER_DESC));
    materialDescriptor.ByteWidth = sizeof(float)* 20;
    materialDescriptor.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    HRESULT hr = device->CreateBuffer(&materialDescriptor, 0, &m_constantBuffers[1]);
    if (FAILED(hr))
    {
        MSG_TRACE_CHANNEL("EFFECT_ERROR", "failed to create the material buffer for this effect: 0x%x(%s)", hr, D3DDebugHelperFunctions::D3DErrorCodeToString(hr));
            return;
    }
    D3DDebugHelperFunctions::SetDebugChildName(m_constantBuffers[1], FormatString("Constant Buffer 1 Technique %s has: vs: %ull, ps: %ull", techniqueName.c_str(), m_vertexShader, m_pixelShader));
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void Technique::setWVPContent(const DeviceManager& deviceManager, const WVPBufferContent& wvpContent) const
{
    deviceManager.getDeviceContext()->UpdateSubresource(m_constantBuffers[0], 0, 0, (void*)&wvpContent, 0, 0);
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void Technique::setMaterialContent(const DeviceManager& deviceManager, const MaterialContent& materialContent) const
{
    deviceManager.getDeviceContext()->UpdateSubresource(m_constantBuffers[1], 0, 0, (void*)&materialContent, 0, 0);
}