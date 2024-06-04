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
#include <Core/StringOperations/HashString.h>
#include "Logging/LoggingMacros.h"


void TryCreatePSO(Resource* resource, PipelineObject& pso)
{
    if (pso.IsValid())
    {
        RenderResourceHelper renderResource = { resource };
        const DeviceManager& deviceManager = renderResource.getResource().getDeviceManager();
        pso.CreatePipelineStateObject(deviceManager.GetDevice());
    }
}



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
    m_resource = resource;
    for (const tinyxml2::XMLElement* childElement = node->FirstChildElement(); childElement != nullptr; childElement = childElement->NextSiblingElement())
    {
        auto elmentHash = Hashing::hashString(childElement->Value());
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
    return getTechnique(Hashing::hashString(techniqueName));
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
    return getTechnique(Hashing::hashString(techniqueName));
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


void Effect::createPSOs()
{
    ID3D12Device6* device = RenderResourceHelper(m_resource).getResource().getDeviceManager().GetDevice();
    //For all techniques create their PSOs
    for (auto& technique : m_techniques)
    {
        technique.second.GetPipelineState().CreatePipelineStateObject(device);
    }
}

bool Effect::IsValid()
{
    bool isValid = !m_techniques.empty();
    for (auto& technique : m_techniques)
    {
        isValid &= technique.second.IsValid();
    }

    return isValid;
}