#include "ShaderCache.h"
#include <fstream>

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
ShaderCache::ShaderCache()
{
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
ShaderCache::~ShaderCache()
{
    cleanup();

}

//-----------------------------------------------------------------------------
//! @brief   
//! @remark
//-----------------------------------------------------------------------------
void ShaderCache::cleanup()
{
    for (auto& shader : m_vertexShaders)
    {
        shader.second.cleanup();
    }
    m_vertexShaders.clear();
    for (auto& shader : m_domainShaders)
    {
        shader.second.cleanup();
    }
    m_domainShaders.clear();
    for (auto& shader : m_hullShaders)
    {
        shader.second.cleanup();
    }
    m_hullShaders.clear();
    for (auto& shader : m_geometryShaders)
    {
        shader.second.cleanup();
    }
    m_geometryShaders.clear();
    for (auto& shader : m_pixelShaders)
    {
        shader.second.cleanup();
    }
    m_pixelShaders.clear();
    for (auto& shader : m_computeShaders)
    {
        shader.second.cleanup();
    }
    m_computeShaders.clear();
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------

const unsigned int ShaderCache::getVertexShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager)
{
    VertexShader shader;
    shader.deserialise(element);
    unsigned int resourceName = hashString(getResourceNameFromFileName(shader.getFileName()));
    if (getVertexShader(resourceName) == nullptr)
    {
        if (shader.createShader(deviceManager))
        {
            m_vertexShaders.emplace(VertexShaderHandle(resourceName, shader));
        }
    }
    
    return resourceName;
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
const unsigned int ShaderCache::getHullShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager)
{
    HullShader shader;
    shader.deserialise(element);
    unsigned int resourceName = hashString(getResourceNameFromFileName(shader.getFileName()));
    if (getHullShader(resourceName) == nullptr)
    {
        if (shader.createShader(deviceManager))
        {
            m_hullShaders.emplace(HullShaderHandle(resourceName, shader));
        }
    }

    return resourceName;
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
const unsigned int ShaderCache::getDomainShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager)
{
    DomainShader shader;
    shader.deserialise(element);
    unsigned int resourceName = hashString(getResourceNameFromFileName(shader.getFileName()));
    if (getDomainShader(resourceName) == nullptr)
    {
        if (shader.createShader(deviceManager))
        {
            m_domainShaders.emplace(DomainShaderHandle(resourceName, shader));
        }
    }

    return resourceName;
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
const unsigned int ShaderCache::getGeometryShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager)
{
    GeometryShader shader;
    shader.deserialise(element);
    unsigned int resourceName = hashString(getResourceNameFromFileName(shader.getFileName()));
    if (getGeometryShader(resourceName) == nullptr)
    {
        if (shader.createShader(deviceManager))
        {
            m_geometryShaders.emplace(GeometryShaderHandle(resourceName, shader));
        }
    }

    return resourceName;
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
const unsigned int ShaderCache::getPixelShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager)
{
    PixelShader shader;
    shader.deserialise(element);
    unsigned int resourceName = hashString(getResourceNameFromFileName(shader.getFileName()));
    if (getPixelShader(resourceName) == nullptr)
    {
        if (shader.createShader(deviceManager))
        {
            m_pixelShaders.emplace(PixelShaderHandle(resourceName, shader));
        }
    }

    return resourceName;
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
const unsigned int  ShaderCache::getComputeShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager)
{
    ComputeShader shader;
    shader.deserialise(element);
    unsigned int resourceName = hashString(getResourceNameFromFileName(shader.getFileName()));
    if (getComputeShader(resourceName) == nullptr)
    {
        if (shader.createShader(deviceManager))
        {
            m_computeShaders.emplace(ComputeShaderHandle(resourceName, shader));
        }
    }

    return resourceName;
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
const VertexShader* ShaderCache::getVertexShader(unsigned int resourceName) const
{
    if (m_vertexShaders.empty())
    {
        return nullptr;
    }

    unsigned int hashedFileName = resourceName;
    VertexShaders::const_iterator it = m_vertexShaders.find(hashedFileName);
    if (it != m_vertexShaders.end())
    {
        return &(it->second);
    }

    return nullptr;
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
const HullShader* ShaderCache::getHullShader(unsigned int resourceName) const
{
    if (m_hullShaders.empty())
    {
        return nullptr;
    }

    unsigned int hashedFileName = resourceName;
    HullShaders::const_iterator it = m_hullShaders.find(hashedFileName);
    if (it != m_hullShaders.end())
    {
        return &(it->second);
    }
    return nullptr;
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
const DomainShader* ShaderCache::getDomainShader(unsigned int resourceName) const
{
    if (m_domainShaders.empty())
    {
        return nullptr;
    }

    unsigned int hashedFileName = resourceName;
    DomainShaders::const_iterator it = m_domainShaders.find(hashedFileName);
    if (it != m_domainShaders.end())
    {
        return &(it->second);
    }
    return nullptr;
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
const GeometryShader* ShaderCache::getGeometryShader(unsigned int resourceName) const
{
    if (m_geometryShaders.empty())
    {
        return nullptr;
    }

    unsigned int hashedFileName = resourceName;
    GeometryShaders::const_iterator it = m_geometryShaders.find(hashedFileName);
    if (it != m_geometryShaders.end())
    {
        return &(it->second);
    }
    return nullptr;
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
const PixelShader* ShaderCache::getPixelShader(unsigned int resourceName) const
{
    if (m_pixelShaders.empty())
    {
        return nullptr;
    }

    unsigned int hashedFileName = resourceName;
    PixelShaders::const_iterator it = m_pixelShaders.find(hashedFileName);
    if (it != m_pixelShaders.end())
    {
        return &(it->second);
    }
    return nullptr;
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
const ComputeShader* ShaderCache::getComputeShader(unsigned int resourceName) const
{
    if (m_computeShaders.empty())
    {
        return nullptr;
    }

    unsigned int hashedFileName = resourceName;
    ComputeShaders::const_iterator it = m_computeShaders.find(hashedFileName);
    if (it != m_computeShaders.end())
    {
        return &(it->second);
    }
    return nullptr;
}

#ifdef DEBUG
//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void ShaderCache::DumpLoadedShaderNames()
{
    for (VertexShaders::const_iterator it = m_vertexShaders.begin(); it != m_vertexShaders.end(); ++it)
    {
        MSG_TRACE_CHANNEL("ShaderCache", "Vertex Shader: %s", it->second.getFileName().c_str());
    }
}
#endif