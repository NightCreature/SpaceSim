#pragma once

#include "Core/StringOperations/StringHelperFunctions.h"
#include "Core/Types/Types.h"
#include "Core/tinyxml2.h"
#include "Graphics/D3D12/ShaderParameter.h"

#include <d3d12.h>
#include <string>
#include <dxcapi.h>
#include <d3d12shader.h>
#include <filesystem>
#include <vector>

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

std::string getShaderPrefix(ShaderType type);

struct CreatedShaderObjects
{
    ID3DBlob* m_shaderObject = nullptr;
    ID3D12ShaderReflection* m_reflectionObject = nullptr;
    ID3DBlob* m_rootSignatureBlob = nullptr;
};

class Shader
{
public:
    Shader() = default;
    void cleanup() { if (m_shaderBlob != nullptr) { m_shaderBlob->Release(); } }

    void deserialise(const tinyxml2::XMLElement* element, ShaderType defaultType);

    bool createShader(const DeviceManager& deviceManager, const ShaderCompiler& compiler);
    const std::string getFileName() const { return m_fileName; }
    ID3DBlob* getShaderBlob() const { return m_shaderBlob; }
    const void* GetShaderBufferPointer() const { return m_shader.m_shaderObject->GetBufferPointer(); }
    size_t GetShaderBufferLength() const { return m_shader.m_shaderObject->GetBufferSize(); }
    const CreatedShaderObjects& GetCompiledShader() const { return m_shader; }
    void SetCompiledShader(const CreatedShaderObjects& shader) { m_shader = shader; }

    const ShaderParameters GetParameters() const { return m_parameters; }

    ShaderType GetType() const { return m_type; }
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