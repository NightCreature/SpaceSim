#include "ShaderCache.h"
#include <fstream>
#include "Core/FileSystem/FileSystem.h"
#include "Core/Profiler/ProfilerMacros.h"
#include "Core/Resource/Resourceable.h"
#include "Core/Paths.h"
#include "Core/Resource/RenderResource.h"
#include "Core/Thread/JobSystem.h"
#include "Loader/ResourceLoadJobs.h"

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
    PROFILE_FUNCTION();
    m_compiler.Initialise(resource);

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


bool LoadOrCompileShaderJob::Execute(ThreadContext* context)
{
    PROFILE_FUNCTION();
    Resource* resource = context->m_renderResource;
    auto shaderPrecompiledPath = resource->m_paths->getEffectShaderPath() / "Shaders/Compiled/";

    auto& shader = m_context.m_shader;
    auto prefix = getShaderPrefix(shader.GetType());

    auto shaderFilePath = std::filesystem::path(shader.getFileName());
    if (shaderFilePath.empty())
    {
        MSG_ERROR_CHANNEL("Shader", "Failed to get shader file name");
        return false;
    }
    auto shaderName = std::filesystem::path(prefix + "_" + shaderFilePath.stem().string() + ".cso");
    auto findShaderName = [shaderName](const auto& path) { return shaderName == path; };
    auto& deviceManager = RenderResourceHelper(resource).getWriteableResource().getDeviceManager();

    auto iterator = std::find_if(std::cbegin(m_context.m_compiledShaderSources), std::cend(m_context.m_compiledShaderSources), findShaderName);
    if (iterator != std::cend(m_context.m_compiledShaderSources))
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

    return shader.createShader(deviceManager, m_context.m_compiler);
}
