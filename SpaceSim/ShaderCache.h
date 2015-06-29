#pragma once

#include "Shaders.h"
#include <map>

class DeviceManager;

class ShaderCache
{
public:
    ShaderCache();
    ~ShaderCache();

    //These should be renamed to shader create functions and return handles
    const unsigned int getVertexShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);
    const unsigned int getHullShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);
    const unsigned int getDomainShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);
    const unsigned int getGeometryShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);
    const unsigned int getPixelShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);
    const unsigned int getComputeShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);

    //This should be taking a shader handle and returning an actual shader object, and only accessed from the renderer.
    const VertexShader* getVertexShader(unsigned int resourceName) const;
    const HullShader* getHullShader(unsigned int resourceName) const;
    const DomainShader* getDomainShader(unsigned int resourceName) const;
    const GeometryShader* getGeometryShader(unsigned int resourceName) const;
    const PixelShader* getPixelShader(unsigned int resourceName) const;
    const ComputeShader* getComputeShader(unsigned int resourceName) const;

#ifdef DEBUG
    void DumpLoadedShaderNames();
#endif
private:

    typedef std::pair<unsigned int, VertexShader>   VertexShaderHandle;
    typedef std::pair<unsigned int, HullShader>     HullShaderHandle;
    typedef std::pair<unsigned int, DomainShader>   DomainShaderHandle;
    typedef std::pair<unsigned int, GeometryShader> GeometryShaderHandle;
    typedef std::pair<unsigned int, PixelShader>    PixelShaderHandle;
    typedef std::pair<unsigned int, ComputeShader>  ComputeShaderHandle;
    typedef std::map<unsigned int, VertexShader>   VertexShaders;
    typedef std::map<unsigned int, HullShader>     HullShaders;
    typedef std::map<unsigned int, DomainShader>   DomainShaders;
    typedef std::map<unsigned int, GeometryShader> GeometryShaders;
    typedef std::map<unsigned int, PixelShader>    PixelShaders;
    typedef std::map<unsigned int, ComputeShader>  ComputeShaders;

    VertexShaders m_vertexShaders;
    HullShaders m_hullShaders;
    DomainShaders m_domainShaders;
    GeometryShaders m_geometryShaders;
    PixelShaders m_pixelShaders;
    ComputeShaders m_computeShaders;
};

