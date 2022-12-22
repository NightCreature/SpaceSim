#pragma once


#include "Graphics/D3D12/ShaderCompiler.h"
#include "Shaders.h"

#include <map>

class DeviceManager;

constexpr size_t InvalidShaderId = static_cast<size_t>(-1);

class ShaderCache
{
public:
    ShaderCache();
    ~ShaderCache();

    void Initialise();
    void cleanup();

    //These should be renamed to shader create functions and return handles
    const size_t getVertexShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);
    const size_t getHullShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);
    const size_t getDomainShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);
    const size_t getGeometryShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);
    const size_t getPixelShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);
    const size_t getComputeShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);

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

    size_t getShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);

    typedef std::pair<size_t, Shader>   VertexShaderHandle;
    typedef std::pair<size_t, Shader>     HullShaderHandle;
    typedef std::pair<size_t, Shader>   DomainShaderHandle;
    typedef std::pair<size_t, Shader> GeometryShaderHandle;
    typedef std::pair<size_t, Shader>    PixelShaderHandle;
    typedef std::pair<size_t, Shader>  ComputeShaderHandle;
    typedef std::map<size_t, Shader>   VertexShaders;
    typedef std::map<size_t, Shader>     HullShaders;
    typedef std::map<size_t, Shader>   DomainShaders;
    typedef std::map<size_t, Shader> GeometryShaders;
    typedef std::map<size_t, Shader>    PixelShaders;
    typedef std::map<size_t, Shader>  ComputeShaders;

    VertexShaders m_vertexShaders;
    HullShaders m_hullShaders;
    DomainShaders m_domainShaders;
    GeometryShaders m_geometryShaders;
    PixelShaders m_pixelShaders;
    ComputeShaders m_computeShaders;

    ShaderCompiler m_compiler;
};

