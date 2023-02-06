#pragma once

#include <d3d12.h>
#include <dxcapi.h>
#include <filesystem>

struct CreatedShaderObjects;

class ShaderCompiler
{
public:

    bool Initialise();

    bool CreateShader(const std::filesystem::path& filePath, const std::wstring& entrypoint, const std::wstring& profileName, size_t compilerFlags, bool createRootDescriptor, CreatedShaderObjects& shaderObject) const;
private:
    IDxcUtils* m_dxcUtils = nullptr;
    IDxcCompiler3* m_compiler = nullptr;
    IDxcIncludeHandler* m_includeHandler = nullptr;
};