#include "Effect.h"
#include "DeviceManager.h"
#include "..\SpaceSim\tinyxml2.h"

#include <assert.h>
#include <D3DCompiler.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "..\SpaceSim\DeviceManager.h"
#include "..\SpaceSim\StringHelperFunctions.h"
#include "..\SpaceSim\D3DDebugHelperFunctions.h"
#include "..\SpaceSim\GameResource.h"
#include "..\SpaceSim\ShaderCache.h"
#include <stdio.h>

HASH_ELEMENT_IMPLEMENTATION(Effect)
HASH_ELEMENT_IMPLEMENTATION(Technique)


//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void Effect::setupEffect() const
{
    //ID3D11DeviceContext* deviceContext = deviceManager.getDeviceContext();
    //deviceContext->VSSetConstantBuffers(0, 1, &m_constantBuffers[0]);
    //deviceContext->PSSetConstantBuffers(0, 1, &m_constantBuffers[1]);
    //deviceContext->VSSetShader( m_vertexShader, NULL, 0 );
    //deviceContext->PSSetShader( m_pixelShader, NULL, 0 );
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void Effect::deserialise(const tinyxml2::XMLElement* node, Resource* resource)
{
    for (const tinyxml2::XMLElement* childElement = node->FirstChildElement(); childElement != nullptr; childElement = childElement->NextSiblingElement())
    {
        unsigned int elmentHash = hashString(childElement->Value());
        if (Technique::m_hash == elmentHash)
        {
            Technique technique(resource);
            technique.deserialise(childElement);
            m_techniques.emplace( std::pair<unsigned int, Technique>(technique.getNameHash(), technique) );
        }
    }
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
const Technique* Effect::getTechnique(const std::string& techniqueName) const
{
    auto it = m_techniques.find(hashString(techniqueName));
    if (it != end(m_techniques))
    {
        return &(it->second);
    }

    return nullptr;
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
const Technique* Effect::getTechnique(const unsigned int techniqueName) const
{
    auto it = m_techniques.find(techniqueName);
    if (it != end(m_techniques))
    {
        return &(it->second);
    }

    return nullptr;
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
void Technique::deserialise(const tinyxml2::XMLElement* element)
{
    GameResource* gameResource = (GameResource*)m_resource;
    ShaderCache* shaderCache = gameResource->getShaderCache();
    const DeviceManager& deviceManager = *gameResource->getDeviceManager();

    const tinyxml2::XMLAttribute* attribute = element->FindAttribute("name");
    if (attribute != nullptr)
    {
        m_name = attribute->Value();
    }
    else
    {
        m_name = "default_technique";
    }
    m_nameHash = hashString(m_name);

    for (const tinyxml2::XMLElement* childElement = element->FirstChildElement(); childElement != nullptr; childElement = childElement->NextSiblingElement())
    {
        unsigned int elmentHash = hashString(childElement->Value());
        if (VertexShader::m_hash == elmentHash)
        {
            m_vertexShader = shaderCache->getVertexShader(childElement, deviceManager);
        }
        else if (HullShader::m_hash == elmentHash)
        {
            m_hullShader = shaderCache->getHullShader(childElement, deviceManager);
        }
        else if (DomainShader::m_hash == elmentHash)
        {
            m_domainShader = shaderCache->getDomainShader(childElement, deviceManager);
        }
        else if (GeometryShader::m_hash == elmentHash)
        {
            m_geometryShader = shaderCache->getGeometryShader(childElement, deviceManager);
        }
        else if (PixelShader::m_hash == elmentHash)
        {
            m_pixelShader = shaderCache->getPixelShader(childElement, deviceManager);
        }
        else if (ComputeShader::m_hash == elmentHash)
        {
            m_computeShader = shaderCache->getComputeShader(childElement, deviceManager);
        }
    }

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
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void Technique::setWVPContent(const DeviceManager& deviceManager, const WVPBufferContent& wvpContent) const
{
    deviceManager.getDeviceContext()->UpdateSubresource(m_constantBuffers[0], 0, 0, (void*)&wvpContent, 0, 0);
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void Technique::setMaterialContent(const DeviceManager& deviceManager, const MaterialContent& materialContent) const
{
    deviceManager.getDeviceContext()->UpdateSubresource(m_constantBuffers[1], 0, 0, (void*)&materialContent, 0, 0);
}