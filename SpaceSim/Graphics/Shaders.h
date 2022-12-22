#pragma once

#include "Core/StringOperations/StringHelperFunctions.h"
#include "Core/Types/Types.h"
#include "Core/tinyxml2.h"
#include "Graphics/D3D12/ShaderParameter.h"

#include <d3d12.h>
#include <string>
#include <dxcapi.h>
#include <d3d12shader.h>

class DeviceManager;
class ShaderCompiler;

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

struct CreatedShaderObjects
{
    IDxcBlob* m_shaderObject;
    ID3D12ShaderReflection* m_reflectionObject;
    IDxcBlob* m_rootSignatureBlob;
};

class Shader
{
public:
    Shader() = default;
    void cleanup() { m_shaderBlob->Release(); }

    void deserialise(const tinyxml2::XMLElement* element, ShaderType defaultType);

    bool createShader(const DeviceManager& deviceManager, const ShaderCompiler& compiler);
    const std::string getFileName() const { return m_fileName; }
    ID3DBlob* getShaderBlob() const { return m_shaderBlob; }
    const void* GetShaderBufferPointer() const { return m_shader.m_shaderObject->GetBufferPointer(); }
    const size_t GetShaderBufferLength() const { return m_shader.m_shaderObject->GetBufferSize(); }
    const CreatedShaderObjects& GetCompiledShader() const { return m_shader; }

    const ShaderParameters GetParameters() const { return m_parameters; }

    HASH_ELEMENT_DEFINITION(Shader);
private:
    CreatedShaderObjects m_shader;

    ID3DBlob* m_shaderBlob;
    std::string m_fileName;
    std::string m_entryPoint;
    std::string m_profileVersion;
    ShaderType m_type;
    ShaderParameters m_parameters;
};