#include "ShaderCache.h"
#include <fstream>
#include "Core/FileSystem/FileSystem.h"
#include "Core/Resource/Resourceable.h"
#include "Core/Paths.h"
#include "Core/Resource/RenderResource.h"
#include <D3Dcompiler.h>
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
void ShaderCache::Initialise(Resource* resource)
{
    m_compiler.Initialise();

    //Build file list of compiled shaders, looking for .cso in /Shaders/Compiled/
    VFS::FileSystem* fileSystem = resource->m_fileSystem;
    m_compiledShaderSources = fileSystem->ListFiles("Shaders/Compiled/");

    m_resource = resource;
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
size_t ShaderCache::getVertexShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager)
{
    UNUSEDPARAM(deviceManager);
    return GetShader(ShaderType::eVertexShader, element, m_vertexShaders);
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
size_t ShaderCache::getHullShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager)
{
    UNUSEDPARAM(deviceManager);
    return GetShader(ShaderType::eHullShader, element, m_hullShaders);
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
size_t ShaderCache::getDomainShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager)
{
    UNUSEDPARAM(deviceManager);
    return GetShader(ShaderType::eDomainShader, element, m_domainShaders);
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
size_t ShaderCache::getGeometryShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager)
{
    UNUSEDPARAM(deviceManager);
    return GetShader(ShaderType::eGeometryShader, element, m_geometryShaders);
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
size_t ShaderCache::getPixelShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager)
{
    UNUSEDPARAM(deviceManager);
    return GetShader(ShaderType::ePixelShader, element, m_pixelShaders);
}

///-------------------------------------------------------------------------
// @brief 
///-------------------------------------------------------------------------
size_t  ShaderCache::getComputeShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager)
{
    UNUSEDPARAM(deviceManager);
    return GetShader(ShaderType::eComputeShader, element, m_computeShaders);
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

size_t ShaderCache::GetShader(ShaderType type, const tinyxml2::XMLElement* element, ShaderMap& shaderMap)
{
    Shader shader;
    shader.deserialise(element, type);
    size_t resourceName = hashString(getResourceNameFromFileName(shader.getFileName()));

    auto shaderIt = shaderMap.find(resourceName);
    if (shaderIt == shaderMap.end())
    {
        if (GetPreCompiledOrCreateShader(shader))
        {
            shaderMap.emplace(std::make_pair(resourceName, shader));
        }
        else
        {
            return InvalidShaderId;
        }
    }

    return resourceName;
}


///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
bool ShaderCache::GetPreCompiledOrCreateShader(Shader& shader)
{
    OPTICK_EVENT();

    auto shaderPrecompiledPath = m_resource->m_paths->getEffectShaderPath() / "Shaders/Compiled/";

    auto prefix = getShaderPrefix(shader.GetType());

    auto shaderFilePath = std::filesystem::path(shader.getFileName());
    auto shaderName = std::filesystem::path(prefix + "_" + shaderFilePath.stem().string() + ".cso");
    auto findShaderName = [shaderName](const auto& path) { return shaderName == path; };
    auto& deviceManager = RenderResourceHelper(m_resource).getWriteableResource().getDeviceManager();

    auto iterator = std::find_if(std::cbegin(m_compiledShaderSources), std::cend(m_compiledShaderSources), findShaderName);
    if (iterator != std::cend(m_compiledShaderSources))
    {
        CreatedShaderObjects shaderObjects;
        
        shaderName = shaderPrecompiledPath / shaderName;
        HRESULT hr = D3DReadFileToBlob(shaderName.wstring().c_str(), &shaderObjects.m_shaderObject);
        if (FAILED(hr))
        {
            MSG_ERROR_CHANNEL("Shader", "Failed to get precompiled shader with error: )x%d, %s", hr, getLastErrorMessage(hr));
        }

        ShaderType type = shader.GetType();
        if (type == ShaderType::eComputeShader || type == ShaderType::eVertexShader)
        {
            //Have to pick up the rootsignature
            auto rootSignatureName = shaderPrecompiledPath / std::filesystem::path(prefix + "_" + shaderFilePath.stem().string() + ".rs");
            hr = D3DReadFileToBlob(rootSignatureName.wstring().c_str(), &shaderObjects.m_rootSignatureBlob);
            if (FAILED(hr))
            {
                MSG_ERROR_CHANNEL("Shader", "Failed to get precompiled shader with error: )x%d, %s", hr, getLastErrorMessage(hr));
            }
        }

        shader.SetCompiledShader(shaderObjects);
        return true;
    }

    return shader.createShader(deviceManager, m_compiler);
}
