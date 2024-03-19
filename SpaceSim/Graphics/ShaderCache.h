#pragma once


#include "Graphics/D3D12/ShaderCompiler.h"
#include "Shaders.h"

#include <map>

class DeviceManager;
class Resource;

constexpr size_t InvalidShaderId = static_cast<size_t>(-1);

class ShaderCache
{
public:
    ShaderCache();
    ~ShaderCache();

    void Initialise(Resource* resource);
    void cleanup();

    //These should be renamed to shader create functions and return handles
    size_t getVertexShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);
    size_t getHullShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);
    size_t getDomainShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);
    size_t getGeometryShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);
    size_t getPixelShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);
    size_t getComputeShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);

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
    size_t GetShader(ShaderType type, const tinyxml2::XMLElement* element, ShaderMap& shaderMap);
    bool GetPreCompiledOrCreateShader(Shader& shader);

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
};

