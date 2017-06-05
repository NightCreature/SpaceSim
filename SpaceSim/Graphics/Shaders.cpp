#include "Graphics/Shaders.h"

#include "Graphics/D3DDebugHelperFunctions.h"
#include "Graphics/DeviceManager.h"
#include "Core/StringOperations/StringHelperFunctions.h"

#include <D3Dcompiler.h>
#include <fstream>

HASH_ELEMENT_IMPLEMENTATION(VertexShader);
HASH_ELEMENT_IMPLEMENTATION(HullShader);
HASH_ELEMENT_IMPLEMENTATION(DomainShader);
HASH_ELEMENT_IMPLEMENTATION(GeometryShader);
HASH_ELEMENT_IMPLEMENTATION(PixelShader);
HASH_ELEMENT_IMPLEMENTATION(ComputeShader);

enum ShaderType
{
    eVertexShader = 0,
    eHullShader,
    eDomainShader,
    eGeometryShader,
    ePixelShader,
    eComputeShader
};

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void getProfileName(const DeviceManager& deviceManager, ShaderType type, std::string &profileName, const std::string profileVersion = std::string())
{
    D3D_FEATURE_LEVEL featureLevel = deviceManager.getFreatureLevel();

    switch (type)
    {
    case eVertexShader:
    {
        profileName = "vs";
    }
    break;
    case eHullShader:
    {
        profileName = "hs";
    }
    break;
    case eDomainShader:
    {
        profileName = "ds";
    }
    break;
    case eGeometryShader:
    {
        profileName = "gs";
    }
    break;
    case ePixelShader:
    {
        profileName = "ps";
    }
    break;
    case eComputeShader:
    {
        profileName = "cs";
    }
    break;
    }
    if (profileVersion.empty())
    {
        switch (featureLevel)
        {
        case D3D_FEATURE_LEVEL_9_1:
        case D3D_FEATURE_LEVEL_9_2:
        {
            profileName += "_4_0_level_9_1";
        }
        break;
        case D3D_FEATURE_LEVEL_9_3:
        {
            profileName += "_4_0_level_9_3";
        }
        break;
        case D3D_FEATURE_LEVEL_10_0:
        {
            profileName += "_4_0";
        }
        break;
        case D3D_FEATURE_LEVEL_10_1:
        {
            profileName += "_4_1";
        }
        break;
        case D3D_FEATURE_LEVEL_11_0:
        case D3D_FEATURE_LEVEL_11_1:
        {
            profileName += "_5_0";
        }
        break;
        default:
        {
            profileName += "_4_0_level_9_1";
        }
        break;
        }
    }
    else
    {
        profileName += profileVersion;
    }
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
char* getShaderBuffer(const std::string& fileName, size_t& length)
{
    std::fstream shaderStream;
    shaderStream.open(fileName, std::fstream::in);
    length = 0;
    char * buffer = nullptr;
    if (shaderStream.is_open())
    {
        shaderStream.seekg(0, std::fstream::end);
        length = shaderStream.tellg();
        shaderStream.seekg(0, std::fstream::beg);

        // allocate memory:
       buffer = new char[length];

        // read data as a block:
        shaderStream.read(buffer, length);
        shaderStream.close();
        char* endChar = buffer + length;
        while (*endChar != '}')
        {
            --endChar;
        }
        ++endChar;
        *endChar = '\0';
    }
    else
    {
        if (shaderStream.fail())
        {
            MSG_TRACE_CHANNEL("SHADER_FILE_READING", "Failed to read %s shader file", fileName.c_str());
        }
    }

    return buffer;
}

void deserialiseSahderNode(const tinyxml2::XMLElement* element, std::string& entryPoint, std::string& profileVersion, std::string& fileName)
{
    for (const tinyxml2::XMLAttribute* attribute = element->FirstAttribute(); attribute != nullptr; attribute = attribute->Next())
    {
        if (strICmp(attribute->Name(), "entry_point"))
        {
            entryPoint = attribute->Value();
        }
        else if (strICmp(attribute->Name(), "profile_version"))
        {
            profileVersion = attribute->Value();
        }
        else if (strICmp(attribute->Name(), "file_name"))
        {
            fileName = attribute->Value();
        }
}
    }


#if defined( DEBUG ) || defined( _DEBUG )
DWORD shaderCompilerFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG;
#else
DWORD shaderCompilerFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
#endif

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void VertexShader::deserialise(const tinyxml2::XMLElement* element)
{
    deserialiseSahderNode(element, m_entryPoint, m_profileVersion, m_fileName);

}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
bool VertexShader::createShader(const DeviceManager& deviceManager)
{
    size_t length = 0;
    char* shaderCodeBuffer = getShaderBuffer(m_fileName, length);
    if (shaderCodeBuffer)
    {
        std::string profileName = "";
        getProfileName(deviceManager, eVertexShader, profileName);
        ID3DBlob* errorBlob;
        HRESULT hr = D3DCompile(shaderCodeBuffer, length, m_fileName.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, m_entryPoint.c_str(), profileName.c_str(), shaderCompilerFlags, 0, &m_vertexShaderBlob, &errorBlob);
        if (FAILED(hr))
        {
            MSG_TRACE_CHANNEL("VertexShader_ERROR", "Failed to compile vertex shader with error code: 0x%x(%s)", hr, D3DDebugHelperFunctions::D3DErrorCodeToString(hr));
            MSG_TRACE_CHANNEL("VertexShader_ERROR", "Failed to compile vertex shader: %s, with errors: \n%s", m_fileName.c_str(), (errorBlob != nullptr ? (char*)errorBlob->GetBufferPointer() : "<errorBlob pointer is nullptr>"));
            delete [] shaderCodeBuffer;
            return false;
        }
        
        hr = deviceManager.getDevice()->CreateVertexShader(m_vertexShaderBlob->GetBufferPointer(), m_vertexShaderBlob->GetBufferSize(), 0, &m_shader);
        if (FAILED(hr))
        {
            MSG_TRACE_CHANNEL("VERTEXSHADER_ERROR", "Failed to create vertex shader: 0x%x(%s)", hr, D3DDebugHelperFunctions::D3DErrorCodeToString(hr));
            delete [] shaderCodeBuffer;
            return false;
        }

        delete []  shaderCodeBuffer;

#ifdef _DEBUG
        if (m_shader != nullptr)
        {
            m_shader->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(m_fileName.size()), m_fileName.c_str());
        }
#endif
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void HullShader::deserialise(const tinyxml2::XMLElement* element)
{
    deserialiseSahderNode(element, m_entryPoint, m_profileVersion, m_fileName);
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
bool HullShader::createShader(const DeviceManager& deviceManager)
{
    D3D_FEATURE_LEVEL featureLevel = deviceManager.getFreatureLevel();
    if (featureLevel < D3D_FEATURE_LEVEL_11_0)
    {
        return true;
    }

    size_t length = 0;
    char* shaderCodeBuffer = getShaderBuffer(m_fileName, length);
    if (shaderCodeBuffer)
    {
        std::string profileName = "";
        getProfileName(deviceManager, eHullShader, profileName, m_profileVersion);
        ID3DBlob* errorBlob;
        ID3DBlob* shaderBlob;
        HRESULT hr = D3DCompile(shaderCodeBuffer, length, m_fileName.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, m_entryPoint.c_str(), profileName.c_str(), shaderCompilerFlags, 0, &shaderBlob, &errorBlob);
        if (FAILED(hr))
        {
            MSG_TRACE_CHANNEL("HullShader_ERROR", "Failed to compile vertex shader with error code: 0x%x(%s)", hr, D3DDebugHelperFunctions::D3DErrorCodeToString(hr));
            MSG_TRACE_CHANNEL("HullShader_ERROR", "Failed to compile vertex shader: %s, with errors: \n%s", m_fileName.c_str(), (errorBlob != nullptr ? (char*)errorBlob->GetBufferPointer() : "<errorBlob pointer is nullptr>"));
            delete[] shaderCodeBuffer;
            return false;
        }

        hr = deviceManager.getDevice()->CreateHullShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &m_shader);
        if (FAILED(hr))
        {
            MSG_TRACE_CHANNEL("VERTEXSHADER_ERROR", "Failed to create vertex shader: 0x%x(%s)", hr, D3DDebugHelperFunctions::D3DErrorCodeToString(hr));
            delete[] shaderCodeBuffer;
            return false;
        }

        delete[]  shaderCodeBuffer;
#ifdef _DEBUG
        if (m_shader != nullptr)
        {
            m_shader->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(m_fileName.size()), m_fileName.c_str());
        }
#endif
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void DomainShader::deserialise(const tinyxml2::XMLElement* element)
{
    deserialiseSahderNode(element, m_entryPoint, m_profileVersion, m_fileName);
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
bool DomainShader::createShader(const DeviceManager& deviceManager)
{
    D3D_FEATURE_LEVEL featureLevel = deviceManager.getFreatureLevel();
    if (featureLevel < D3D_FEATURE_LEVEL_11_0)
    {
        return true;
    }

    size_t length = 0;
    char* shaderCodeBuffer = getShaderBuffer(m_fileName, length);
    if (shaderCodeBuffer)
    {
        std::string profileName = "";
        getProfileName(deviceManager, eDomainShader, profileName, m_profileVersion);
        ID3DBlob* errorBlob;
        ID3DBlob* shaderBlob;
        HRESULT hr = D3DCompile(shaderCodeBuffer, length, m_fileName.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, m_entryPoint.c_str(), profileName.c_str(), shaderCompilerFlags, 0, &shaderBlob, &errorBlob);
        if (FAILED(hr))
        {
            MSG_TRACE_CHANNEL("DomainShader_ERROR", "Failed to compile vertex shader with error code: 0x%x(%s)", hr, D3DDebugHelperFunctions::D3DErrorCodeToString(hr));
            MSG_TRACE_CHANNEL("DomainShader_ERROR", "Failed to compile vertex shader: %s, with errors: \n%s", m_fileName.c_str(), (errorBlob != nullptr ? (char*)errorBlob->GetBufferPointer() : "<errorBlob pointer is nullptr>"));
            delete[] shaderCodeBuffer;
            return false;
        }

        hr = deviceManager.getDevice()->CreateDomainShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &m_shader);
        if (FAILED(hr))
        {
            MSG_TRACE_CHANNEL("VERTEXSHADER_ERROR", "Failed to create vertex shader: 0x%x(%s)", hr, D3DDebugHelperFunctions::D3DErrorCodeToString(hr));
            delete[] shaderCodeBuffer;
            return false;
        }

        delete[]  shaderCodeBuffer;
#ifdef _DEBUG
        if (m_shader != nullptr)
        {
            m_shader->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(m_fileName.size()), m_fileName.c_str());
        }
#endif
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void GeometryShader::deserialise(const tinyxml2::XMLElement* element)
{
    deserialiseSahderNode(element, m_entryPoint, m_profileVersion, m_fileName);
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
bool GeometryShader::createShader(const DeviceManager& deviceManager)
{
    D3D_FEATURE_LEVEL featureLevel = deviceManager.getFreatureLevel();
    if (featureLevel < D3D_FEATURE_LEVEL_10_0)
    {
        return true;
    }


    size_t length = 0;
    char* shaderCodeBuffer = getShaderBuffer(m_fileName, length);
    if (shaderCodeBuffer)
    {
        std::string profileName = "";
        getProfileName(deviceManager, eGeometryShader, profileName, m_profileVersion);
        ID3DBlob* errorBlob;
        ID3DBlob* shaderBlob;
        HRESULT hr = D3DCompile(shaderCodeBuffer, length, m_fileName.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, m_entryPoint.c_str(), profileName.c_str(), shaderCompilerFlags, 0, &shaderBlob, &errorBlob);
        if (FAILED(hr))
        {
            MSG_TRACE_CHANNEL("GeometryShader_ERROR", "Failed to compile vertex shader with error code: 0x%x(%s)", hr, D3DDebugHelperFunctions::D3DErrorCodeToString(hr));
            MSG_TRACE_CHANNEL("GeometryShader_ERROR", "Failed to compile vertex shader: %s, with errors: \n%s", m_fileName.c_str(), (errorBlob != nullptr ? (char*)errorBlob->GetBufferPointer() : "<errorBlob pointer is nullptr>"));
            delete[] shaderCodeBuffer;
            return false;
        }

        hr = deviceManager.getDevice()->CreateGeometryShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &m_shader);
        if (FAILED(hr))
        {
            MSG_TRACE_CHANNEL("VERTEXSHADER_ERROR", "Failed to create vertex shader: 0x%x(%s)", hr, D3DDebugHelperFunctions::D3DErrorCodeToString(hr));
            delete[] shaderCodeBuffer;
            return false;
        }

        delete[]  shaderCodeBuffer;
#ifdef _DEBUG
        if (m_shader != nullptr)
        {
            m_shader->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(m_fileName.size()), m_fileName.c_str());
        }
#endif
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void PixelShader::deserialise(const tinyxml2::XMLElement* element)
{
    deserialiseSahderNode(element, m_entryPoint, m_profileVersion, m_fileName);
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
bool PixelShader::createShader(const DeviceManager& deviceManager)
{
    size_t length = 0;
    char* shaderCodeBuffer = getShaderBuffer(m_fileName, length);
    if (shaderCodeBuffer)
    {
        std::string profileName = "";
        getProfileName(deviceManager, ePixelShader, profileName, m_profileVersion);
        ID3DBlob* errorBlob;
        ID3DBlob* shaderBlob;
        HRESULT hr = D3DCompile(shaderCodeBuffer, length, m_fileName.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, m_entryPoint.c_str(), profileName.c_str(), shaderCompilerFlags, 0, &shaderBlob, &errorBlob);
        if (FAILED(hr))
        {
            MSG_TRACE_CHANNEL("PixelShader_ERROR", "Failed to compile vertex shader with error code: 0x%x(%s)", hr, D3DDebugHelperFunctions::D3DErrorCodeToString(hr));
            MSG_TRACE_CHANNEL("PixelShader_ERROR", "Failed to compile vertex shader: %s, with errors: \n%s", m_fileName.c_str(), (errorBlob != nullptr ? (char*)errorBlob->GetBufferPointer() : "<errorBlob pointer is nullptr>"));
            delete[] shaderCodeBuffer;
            return false;
        }

        hr = deviceManager.getDevice()->CreatePixelShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &m_shader);
        if (FAILED(hr))
        {
            MSG_TRACE_CHANNEL("VERTEXSHADER_ERROR", "Failed to create vertex shader: 0x%x(%s)", hr, D3DDebugHelperFunctions::D3DErrorCodeToString(hr));
            delete[] shaderCodeBuffer;
            return false;
        }

        delete[]  shaderCodeBuffer;
#ifdef _DEBUG
        if (m_shader != nullptr)
        {
            m_shader->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(m_fileName.size()), m_fileName.c_str());
        }
#endif
        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
void ComputeShader::deserialise(const tinyxml2::XMLElement* element)
{
    deserialiseSahderNode(element, m_entryPoint, m_profileVersion, m_fileName);
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
bool ComputeShader::createShader(const DeviceManager& deviceManager)
{
    D3D_FEATURE_LEVEL featureLevel = deviceManager.getFreatureLevel();
    if (featureLevel < D3D_FEATURE_LEVEL_11_0)
    {
        return true;
    }

    size_t length = 0;
    char* shaderCodeBuffer = getShaderBuffer(m_fileName, length);
    if (shaderCodeBuffer)
    {
        std::string profileName = "";
        getProfileName(deviceManager, eComputeShader, profileName, m_profileVersion);
        ID3DBlob* errorBlob;
        ID3DBlob* shaderBlob;
        HRESULT hr = D3DCompile(shaderCodeBuffer, length, m_fileName.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, m_entryPoint.c_str(), profileName.c_str(), shaderCompilerFlags, 0, &shaderBlob, &errorBlob);
        if (FAILED(hr))
        {
            MSG_TRACE_CHANNEL("ComputeShader_ERROR", "Failed to compile vertex shader with error code: 0x%x(%s)", hr, D3DDebugHelperFunctions::D3DErrorCodeToString(hr));
            MSG_TRACE_CHANNEL("ComputeShader_ERROR", "Failed to compile vertex shader: %s, with errors: \n%s", m_fileName.c_str(), (errorBlob != nullptr ? (char*)errorBlob->GetBufferPointer() : "<errorBlob pointer is nullptr>"));
            delete[] shaderCodeBuffer;
            return false;
        }

        hr = deviceManager.getDevice()->CreateComputeShader(shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(), 0, &m_shader);
        if (FAILED(hr))
        {
            MSG_TRACE_CHANNEL("VERTEXSHADER_ERROR", "Failed to create vertex shader: 0x%x(%s)", hr, D3DDebugHelperFunctions::D3DErrorCodeToString(hr));
            delete[] shaderCodeBuffer;
            return false;
        }

        delete[]  shaderCodeBuffer;
#ifdef _DEBUG
        if (m_shader != nullptr)
        {
            m_shader->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(m_fileName.size()), m_fileName.c_str());
        }
#endif
        return true;
    }

    return false;
}