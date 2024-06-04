#pragma once

#include "Core/Thread/Job.h"
#include "Graphics/D3D12/ShaderCompiler.h"
#include "Shaders.h"

#include <map>
#include <functional>
#include "Core/Profiler/ProfilerMacros.h"
#include "Core/Thread/JobSystem.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include <mutex>


class DeviceManager;
class Resource;

constexpr size_t InvalidShaderId = static_cast<size_t>(-1);

class LoadOrCompileShaderJob : public Job
{
public:
    struct Context
    {
        Context(Shader& shader, const std::vector<std::filesystem::path>& compiledSources, ShaderCompiler& compiler) : m_shader(shader), m_compiledShaderSources(compiledSources), m_compiler(compiler) {}
        Shader& m_shader;
        const std::vector<std::filesystem::path>& m_compiledShaderSources;
        ShaderCompiler& m_compiler;
    };

    LoadOrCompileShaderJob(Context context, auto callback) : m_context(context), m_callback(callback) {}
    bool Execute(ThreadContext* context) override;

    void Finish(ThreadContext* context) override 
    {
        UNUSEDPARAM(context);
        m_callback(m_context.m_shader);
    }
private:
    Context m_context;
    std::function<void(Shader& shader)> m_callback;
};



class ShaderCache
{
public:
    ShaderCache();
    ~ShaderCache();

    void Initialise(Resource* resource);
    void cleanup();

    //These should be renamed to shader create functions and return handles
    ///-------------------------------------------------------------------------
    // @brief 
    ///-------------------------------------------------------------------------
    size_t getVertexShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager, auto callback)
    {
        UNUSEDPARAM(deviceManager);
        return GetShader(ShaderType::eVertexShader, element, m_vertexShaders, callback);
    }

    ///-------------------------------------------------------------------------
    // @brief 
    ///-------------------------------------------------------------------------
    size_t getHullShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager, auto callback)
    {
        UNUSEDPARAM(deviceManager);
        return GetShader(ShaderType::eHullShader, element, m_hullShaders, callback);
    }

    ///-------------------------------------------------------------------------
    // @brief 
    ///-------------------------------------------------------------------------
    size_t getDomainShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager, auto callback)
    {
        UNUSEDPARAM(deviceManager);
        return GetShader(ShaderType::eDomainShader, element, m_domainShaders, callback);
    }

    ///-------------------------------------------------------------------------
    // @brief 
    ///-------------------------------------------------------------------------
    size_t getGeometryShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager, auto callback)
    {
        UNUSEDPARAM(deviceManager);
        return GetShader(ShaderType::eGeometryShader, element, m_geometryShaders, callback);
    }

    ///-------------------------------------------------------------------------
    // @brief 
    ///-------------------------------------------------------------------------
    size_t getPixelShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager, auto callback)
    {
        UNUSEDPARAM(deviceManager);
        return GetShader(ShaderType::ePixelShader, element, m_pixelShaders, callback);
    }

    ///-------------------------------------------------------------------------
    // @brief 
    ///-------------------------------------------------------------------------
    size_t  getComputeShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager, auto callback)
    {
        UNUSEDPARAM(deviceManager);
        return GetShader(ShaderType::eComputeShader, element, m_computeShaders, callback);
    }

    //This should be taking a shader handle and returning an actual shader object, and only accessed from the renderer.
    const Shader* getVertexShader(size_t resourceName) const;
    const Shader* getHullShader(size_t resourceName) const;
    const Shader* getDomainShader(size_t resourceName) const;
    const Shader* getGeometryShader(size_t resourceName) const;
    const Shader* getPixelShader(size_t resourceName) const;
    const Shader* getComputeShader(size_t resourceName) const;

    const ShaderCompiler GetCompiler() const { return m_compiler; }

#ifdef DEBUG
    void DumpLoadedShaderNames();
#endif
private:
    using ShaderMap = std::map<size_t, Shader>;
    size_t GetShader(ShaderType type, const tinyxml2::XMLElement* element, ShaderMap& shaderMap, auto callback)
    {
        PROFILE_FUNCTION();
        Shader shader;
        shader.deserialise(element, type); //this is happening regardless not great
        size_t resourceName = Hashing::hashString(getResourceNameFromFileName(shader.getFileName()));

        std::scoped_lock lock(m_mutex);
        auto shaderIt = shaderMap.find(resourceName);
        if (shaderIt == shaderMap.end())
        {
            //Create the shader
            GetPreCompiledOrCreateShader(shader, callback);
            //This access is multi threaded now
            shaderMap[resourceName] = shader;
            
        }

        return resourceName;
    }
    bool GetPreCompiledOrCreateShader(Shader& shader, auto callback)
    {
        PROFILE_FUNCTION();

        LoadOrCompileShaderJob::Context context(shader, m_compiledShaderSources, m_compiler);
        LoadOrCompileShaderJob* job = new LoadOrCompileShaderJob(context, callback);

        //auto& resourceHelper = RenderResourceHelper(m_resource).getWriteableResource();
        //resourceHelper.getJobQueue().AddJob(job);
        ThreadContext threadContext;
        threadContext.m_renderResource = m_resource;
        job->Execute(&threadContext);
        job->Finish(&threadContext);
        delete job;

        return true;//Job is scheduled
    }

    using ShaderHandle = std::pair<size_t, Shader>;
    typedef ShaderHandle   VertexShaderHandle;
    typedef ShaderHandle     HullShaderHandle;
    typedef ShaderHandle   DomainShaderHandle;
    typedef ShaderHandle GeometryShaderHandle;
    typedef ShaderHandle    PixelShaderHandle;
    typedef ShaderHandle  ComputeShaderHandle;
    typedef ShaderMap   VertexShaders;
    typedef ShaderMap     HullShaders;
    typedef ShaderMap   DomainShaders;
    typedef ShaderMap GeometryShaders;
    typedef ShaderMap    PixelShaders;
    typedef ShaderMap  ComputeShaders;

    Resource* m_resource = nullptr;
    VertexShaders m_vertexShaders;
    HullShaders m_hullShaders;
    DomainShaders m_domainShaders;
    GeometryShaders m_geometryShaders;
    PixelShaders m_pixelShaders;
    ComputeShaders m_computeShaders;

    ShaderCompiler m_compiler;
    std::vector<std::filesystem::path> m_compiledShaderSources;
    std::mutex m_mutex;
};
