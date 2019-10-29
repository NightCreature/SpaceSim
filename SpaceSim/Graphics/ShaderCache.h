#pragma once

#include "Shaders.h"
#include <map>

class DeviceManager;

class ShaderCache
{
public:
    ShaderCache();
    ~ShaderCache();

    void cleanup();

    //These should be renamed to shader create functions and return handles
    const size_t getVertexShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);
    const size_t getHullShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);
    const size_t getDomainShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);
    const size_t getGeometryShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);
    const size_t getPixelShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);
    const size_t getComputeShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);

    //This should be taking a shader handle and returning an actual shader object, and only accessed from the renderer.
    const VertexShader* getVertexShader(size_t resourceName) const;
    const HullShader* getHullShader(size_t resourceName) const;
    const DomainShader* getDomainShader(size_t resourceName) const;
    const GeometryShader* getGeometryShader(size_t resourceName) const;
    const PixelShader* getPixelShader(size_t resourceName) const;
    const ComputeShader* getComputeShader(size_t resourceName) const;

#ifdef DEBUG
    void DumpLoadedShaderNames();
#endif
private:

    typedef std::pair<size_t, VertexShader>   VertexShaderHandle;
    typedef std::pair<size_t, HullShader>     HullShaderHandle;
    typedef std::pair<size_t, DomainShader>   DomainShaderHandle;
    typedef std::pair<size_t, GeometryShader> GeometryShaderHandle;
    typedef std::pair<size_t, PixelShader>    PixelShaderHandle;
    typedef std::pair<size_t, ComputeShader>  ComputeShaderHandle;
    typedef std::map<size_t, VertexShader>   VertexShaders;
    typedef std::map<size_t, HullShader>     HullShaders;
    typedef std::map<size_t, DomainShader>   DomainShaders;
    typedef std::map<size_t, GeometryShader> GeometryShaders;
    typedef std::map<size_t, PixelShader>    PixelShaders;
    typedef std::map<size_t, ComputeShader>  ComputeShaders;

    VertexShaders m_vertexShaders;
    HullShaders m_hullShaders;
    DomainShaders m_domainShaders;
    GeometryShaders m_geometryShaders;
    PixelShaders m_pixelShaders;
    ComputeShaders m_computeShaders;
};

