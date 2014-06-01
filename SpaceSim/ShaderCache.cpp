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
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------

const VertexShader* ShaderCache::getVertexShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager)
{
    VertexShader shader;
    shader.deserialise(element);
    std::string resourceName = getResourceNameFromFileName(shader.getFileName());
    if (getVertexShader(resourceName) == nullptr)
    {
        if (shader.createShader(deviceManager))
        {
            m_vertexShaders.emplace(VertexShaderHandle(hashString(resourceName), shader));
        }
    }
    
    return getVertexShader(resourceName);
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
const HullShader* ShaderCache::getHullShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager)
{
    HullShader shader;
    shader.deserialise(element);
    std::string resourceName = getResourceNameFromFileName(shader.getFileName());
    if (getHullShader(resourceName) == nullptr)
    {
        if (shader.createShader(deviceManager))
        {
            m_hullShaders.emplace(HullShaderHandle(hashString(resourceName), shader));
        }
    }

    return getHullShader(resourceName);
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
const DomainShader* ShaderCache::getDomainShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager)
{
    DomainShader shader;
    shader.deserialise(element);
    std::string resourceName = getResourceNameFromFileName(shader.getFileName());
    if (getDomainShader(resourceName) == nullptr)
    {
        if (shader.createShader(deviceManager))
        {
            m_domainShaders.emplace(DomainShaderHandle(hashString(resourceName), shader));
        }
    }

    return getDomainShader(resourceName);
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
const GeometryShader* ShaderCache::getGeometryShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager)
{
    GeometryShader shader;
    shader.deserialise(element);
    std::string resourceName = getResourceNameFromFileName(shader.getFileName());
    if (getGeometryShader(resourceName) == nullptr)
    {
        if (shader.createShader(deviceManager))
        {
            m_geometryShaders.emplace(GeometryShaderHandle(hashString(resourceName), shader));
        }
    }

    return getGeometryShader(resourceName);
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
const PixelShader* ShaderCache::getPixelShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager)
{
    PixelShader shader;
    shader.deserialise(element);
    std::string resourceName = getResourceNameFromFileName(shader.getFileName());
    if (getPixelShader(resourceName) == nullptr)
    {
        if (shader.createShader(deviceManager))
        {
            m_pixelShaders.emplace(PixelShaderHandle(hashString(resourceName), shader));
        }
    }

    return getPixelShader(resourceName);
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
const ComputeShader*  ShaderCache::getComputeShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager)
{
    ComputeShader shader;
    shader.deserialise(element);
    std::string resourceName = getResourceNameFromFileName(shader.getFileName());
    if (getComputeShader(resourceName) == nullptr)
    {
        if (shader.createShader(deviceManager))
        {
            m_computeShaders.emplace(ComputeShaderHandle(hashString(resourceName), shader));
        }
    }

    return getComputeShader(resourceName);
}

//-------------------------------------------------------------------------
// @brief 
//-------------------------------------------------------------------------
const VertexShader* ShaderCache::getVertexShader(const std::string& resourceName)
{
    unsigned int hashedFileName = hashString(resourceName);
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
const HullShader* ShaderCache::getHullShader(const std::string& resourceName)
{
    unsigned int hashedFileName = hashString(resourceName);
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
const DomainShader* ShaderCache::getDomainShader(const std::string& resourceName)
{
    unsigned int hashedFileName = hashString(resourceName);
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
const GeometryShader* ShaderCache::getGeometryShader(const std::string& resourceName)
{
    unsigned int hashedFileName = hashString(resourceName);
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
const PixelShader* ShaderCache::getPixelShader(const std::string& resourceName)
{
    unsigned int hashedFileName = hashString(resourceName);
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
const ComputeShader* ShaderCache::getComputeShader(const std::string& resourceName)
{
    unsigned int hashedFileName = hashString(resourceName);
    ComputeShaders::const_iterator it = m_computeShaders.find(hashedFileName);
    if (it != m_computeShaders.end())
    {
        return &(it->second);
    }
    return nullptr;
}