#pragma once

#include "Shaders.h"
#include <map>

class DeviceManager;

class ShaderCache
{
public:
    ShaderCache();
    ~ShaderCache();

    const VertexShader* getVertexShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);
    const HullShader* getHullShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);
    const DomainShader* getDomainShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);
    const GeometryShader* getGeometryShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);
    const PixelShader* getPixelShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);
    const ComputeShader* getComputeShader(const tinyxml2::XMLElement* element, const DeviceManager& deviceManager);

    const VertexShader* getVertexShader(const std::string& resourceName);
    const HullShader* getHullShader(const std::string& resourceName);
    const DomainShader* getDomainShader(const std::string& resourceName);
    const GeometryShader* getGeometryShader(const std::string& resourceName);
    const PixelShader* getPixelShader(const std::string& resourceName);
    const ComputeShader* getComputeShader(const std::string& resourceName);
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

