#pragma once

#include "Core/StringOperations/StringHelperFunctions.h"
#include "Core/Types/Types.h"
#include "Core/tinyxml2.h"
#include "Graphics/D3D12/ShaderParameter.h"

#include <d3d11.h>
#include <string>

class DeviceManager;

enum class ShaderType : uint8
{
    eVertexShader = 0,
    eHullShader,
    eDomainShader,
    eGeometryShader,
    ePixelShader,
    eComputeShader,

    Count
};

class Shader
{
public:
    Shader() = default;
    void cleanup() { m_shaderBlob->Release(); }

    void deserialise(const tinyxml2::XMLElement* element, ShaderType defaultType);

    bool createShader(const DeviceManager& deviceManager);
    const std::string getFileName() const { return m_fileName; }
    ID3DBlob* getShaderBlob() const { return m_shaderBlob; }

    const ShaderParameters GetParameters() const { return m_parameters; }

    HASH_ELEMENT_DEFINITION(Shader);
private:
    ID3DBlob* m_shaderBlob;
    std::string m_fileName;
    std::string m_entryPoint;
    std::string m_profileVersion;
    ShaderType m_type;
    ShaderParameters m_parameters;
};