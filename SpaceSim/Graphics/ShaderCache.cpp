#include "ShaderCache.h"
#include <fstream>

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
ShaderCache::ShaderCache()
{
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
ShaderCache::~ShaderCache()
{
    cleanup();

}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
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

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------

const size_t ShaderCache::getVertexShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager)
{
    Shader shader;
    shader.deserialise(element, ShaderType::eVertexShader);
    size_t resourceName = hashString(getResourceNameFromFileName(shader.getFileName()));
    if (getVertexShader(resourceName) == nullptr)
    {
        if (shader.createShader(deviceManager))
        {
            m_vertexShaders.emplace(VertexShaderHandle(resourceName, shader));
        }
    }
    
    return resourceName;
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
const size_t ShaderCache::getHullShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager)
{
    Shader shader;
    shader.deserialise(element, ShaderType::eHullShader);
    size_t resourceName = hashString(getResourceNameFromFileName(shader.getFileName()));
    if (getHullShader(resourceName) == nullptr)
    {
        if (shader.createShader(deviceManager))
        {
            m_hullShaders.emplace(HullShaderHandle(resourceName, shader));
        }
    }

    return resourceName;
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
const size_t ShaderCache::getDomainShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager)
{
    Shader shader;
    shader.deserialise(element, ShaderType::eDomainShader);
    size_t resourceName = hashString(getResourceNameFromFileName(shader.getFileName()));
    if (getDomainShader(resourceName) == nullptr)
    {
        if (shader.createShader(deviceManager))
        {
            m_domainShaders.emplace(DomainShaderHandle(resourceName, shader));
        }
    }

    return resourceName;
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
const size_t ShaderCache::getGeometryShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager)
{
    Shader shader;
    shader.deserialise(element, ShaderType::eGeometryShader);
    size_t resourceName = hashString(getResourceNameFromFileName(shader.getFileName()));
    if (getGeometryShader(resourceName) == nullptr)
    {
        if (shader.createShader(deviceManager))
        {
            m_geometryShaders.emplace(GeometryShaderHandle(resourceName, shader));
        }
    }

    return resourceName;
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
const size_t ShaderCache::getPixelShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager)
{
    Shader shader;
    shader.deserialise(element, ShaderType::ePixelShader);
    size_t resourceName = hashString(getResourceNameFromFileName(shader.getFileName()));
    if (getPixelShader(resourceName) == nullptr)
    {
        if (shader.createShader(deviceManager))
        {
            m_pixelShaders.emplace(PixelShaderHandle(resourceName, shader));
        }
    }

    return resourceName;
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
const size_t  ShaderCache::getComputeShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager)
{
    Shader shader;
    shader.deserialise(element, ShaderType::eComputeShader);
    size_t resourceName = hashString(getResourceNameFromFileName(shader.getFileName()));
    if (getComputeShader(resourceName) == nullptr)
    {
        if (shader.createShader(deviceManager))
        {
            m_computeShaders.emplace(ComputeShaderHandle(resourceName, shader));
        }
    }

    return resourceName;
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
const Shader* ShaderCache::getVertexShader(size_t resourceName) const
{
    if (m_vertexShaders.empty())
    {
        return nullptr;
    }

    size_t hashedFileName = resourceName;
    VertexShaders::const_iterator it = m_vertexShaders.find(hashedFileName);
    if (it != m_vertexShaders.end())
    {
        return &(it->second);
    }

    return nullptr;
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
const Shader* ShaderCache::getHullShader(size_t resourceName) const
{
    if (m_hullShaders.empty())
    {
        return nullptr;
    }

    size_t hashedFileName = resourceName;
    HullShaders::const_iterator it = m_hullShaders.find(hashedFileName);
    if (it != m_hullShaders.end())
    {
        return &(it->second);
    }
    return nullptr;
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
const Shader* ShaderCache::getDomainShader(size_t resourceName) const
{
    if (m_domainShaders.empty())
    {
        return nullptr;
    }

    size_t hashedFileName = resourceName;
    DomainShaders::const_iterator it = m_domainShaders.find(hashedFileName);
    if (it != m_domainShaders.end())
    {
        return &(it->second);
    }
    return nullptr;
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
const Shader* ShaderCache::getGeometryShader(size_t resourceName) const
{
    if (m_geometryShaders.empty())
    {
        return nullptr;
    }

    size_t hashedFileName = resourceName;
    GeometryShaders::const_iterator it = m_geometryShaders.find(hashedFileName);
    if (it != m_geometryShaders.end())
    {
        return &(it->second);
    }
    return nullptr;
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
const Shader* ShaderCache::getPixelShader(size_t resourceName) const
{
    if (m_pixelShaders.empty())
    {
        return nullptr;
    }

    size_t hashedFileName = resourceName;
    PixelShaders::const_iterator it = m_pixelShaders.find(hashedFileName);
    if (it != m_pixelShaders.end())
    {
        return &(it->second);
    }
    return nullptr;
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
const Shader* ShaderCache::getComputeShader(size_t resourceName) const
{
    if (m_computeShaders.empty())
    {
        return nullptr;
    }

    size_t hashedFileName = resourceName;
    ComputeShaders::const_iterator it = m_computeShaders.find(hashedFileName);
    if (it != m_computeShaders.end())
    {
        return &(it->second);
    }
    return nullptr;
}

#ifdef DEBUG
///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void ShaderCache::DumpLoadedShaderNames()
{
    for (VertexShaders::const_iterator it = m_vertexShaders.begin(); it != m_vertexShaders.end(); ++it)
    {
        MSG_TRACE_CHANNEL("ShaderCache", "Vertex Shader: %s", it->second.getFileName().c_str());
    }
}

#endif