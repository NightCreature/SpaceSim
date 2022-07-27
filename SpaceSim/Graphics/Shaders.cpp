#include "Graphics/Shaders.h"

#include "Graphics/D3DDebugHelperFunctions.h"
#include "Graphics/DeviceManager.h"
#include "Core/StringOperations/StringHelperFunctions.h"

#include <D3Dcompiler.h>
#include <fstream>
#include <numeric>
#include <limits>
#include "D3D12/D3D12X.h"
#include "D3D12/ShaderParamMatcher.h"

const char* shaderTypeNames[] =
{
    "VS",
    "HS",
    "DS",
    "GS",
    "PS",
    "CS"
};

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void getProfileName(const DeviceManager& deviceManager, ShaderType type, std::string &profileName, const std::string profileVersion = std::string())
{
    D3D_FEATURE_LEVEL featureLevel = deviceManager.getFreatureLevel();

    switch (type)
    {
    case ShaderType::eVertexShader:
    {
        profileName = "vs";
    }
    break;
    case ShaderType::eHullShader:
    {
        profileName = "hs";
    }
    break;
    case ShaderType::eDomainShader:
    {
        profileName = "ds";
    }
    break;
    case ShaderType::eGeometryShader:
    {
        profileName = "gs";
    }
    break;
    case ShaderType::ePixelShader:
    {
        profileName = "ps";
    }
    break;
    case ShaderType::eComputeShader:
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
        case D3D_FEATURE_LEVEL_12_0:
        case D3D_FEATURE_LEVEL_12_1:
        case D3D_FEATURE_LEVEL_1_0_CORE:
        {
            profileName += "_5_1";//This should allow use to use new features, have to see if we need to chgange this to 6_6 or 6_4 and how we determine that need to include the new shader ocmpiler first and its bridge https://github.com/microsoft/DirectXShaderCompiler
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

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
char* getShaderBuffer(const std::string& fileName, size_t& length)
{
    std::fstream shaderStream;
    shaderStream.open(fileName, std::fstream::in);
    length = 0;
    char * buffer = nullptr;
    if (shaderStream.is_open())
    {
        shaderStream.ignore(std::numeric_limits<std::streamsize>::max());
        length = shaderStream.gcount();
        shaderStream.clear();   //  Since ignore will have set eof.
        shaderStream.seekg(0, std::ios_base::beg);

        // allocate memory:
       buffer = new char[length];

       // read data as a block:
        shaderStream.read(buffer, length);
        shaderStream.close();
        char* endChar = buffer + length;
        while (*endChar != '}')
        {
            --endChar;
            --length;
        }
        ++length; //Found last } so add 1 to be at the end of the shader code
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

void deserialiseSahderNode(const tinyxml2::XMLElement* element, std::string& entryPoint, std::string& profileVersion, std::string& fileName, ShaderType& type)
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
        else if (strICmp(attribute->Name(), "shader_type"))
        {
            for (size_t counter = 0; counter < static_cast<std::underlying_type_t<ShaderType>>(ShaderType::Count); ++counter)
            {
                if (strICmp(shaderTypeNames[counter], attribute->Value()))
                {
                    type = static_cast<ShaderType>(counter);
                }
            }
        }
    }
}


#if defined( DEBUG ) || defined( _DEBUG )
DWORD shaderCompilerFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR | D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG | D3DCOMPILE_ALL_RESOURCES_BOUND;
#else
DWORD shaderCompilerFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR | D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES;
#endif



///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void Shader::deserialise(const tinyxml2::XMLElement* element, ShaderType defaultType)
{
    m_type = defaultType;
    deserialiseSahderNode(element, m_entryPoint, m_profileVersion, m_fileName, m_type);
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
bool Shader::createShader(const DeviceManager& deviceManager)
{
    size_t length = 0;
    char* shaderCodeBuffer = getShaderBuffer(m_fileName, length);
    if (shaderCodeBuffer)
    {
        std::string profileName = "";
        getProfileName(deviceManager, m_type, profileName);

        ID3DBlob* errorBlob;
        HRESULT hr = D3DCompile(shaderCodeBuffer, length, m_fileName.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, m_entryPoint.c_str(), profileName.c_str(), shaderCompilerFlags, 0, &m_shaderBlob, &errorBlob);
        if (FAILED(hr))
        {
            MSG_TRACE_CHANNEL("Shader_ERROR", "Failed to compile vertex shader with error code: 0x%x(%s)", hr, getLastErrorMessage(hr));
            MSG_TRACE_CHANNEL("Shader_ERROR", "Failed to compile vertex shader: %s, with errors: \n%s", m_fileName.c_str(), (errorBlob != nullptr ? (char*)errorBlob->GetBufferPointer() : "<errorBlob pointer is nullptr>"));
            delete[] shaderCodeBuffer;
            return false;
        }

        //void* blobBufferPtr = m_shaderBlob->GetBufferPointer();

        //ID3DBlob* rootsignatureBlob = nullptr;
        //hr = D3DGetBlobPart(shaderCodeBuffer, length, D3D_BLOB_ROOT_SIGNATURE, 0, &rootsignatureBlob);
        //if (FAILED(hr))
        //{
        //    MSG_TRACE_CHANNEL("VertexShader_ERROR", "Failed to compile vertex shader with error code: 0x%x(%s)", hr, D3DDebugHelperFunctions::D3DErrorCodeToString(hr));
        //    MSG_TRACE_CHANNEL("VertexShader_ERROR", "Failed to compile vertex shader: %s, with errors: \n%s", m_fileName.c_str(), (errorBlob != nullptr ? (char*)errorBlob->GetBufferPointer() : "<errorBlob pointer is nullptr>"));
        //    delete[] shaderCodeBuffer;
        //    return false;
        //}
        //Pixel shaders currently dont have a RootDescripter attached this is handled at the technique level really
        //if (m_type == ShaderType::eVertexShader)
        {

        }

        delete[] shaderCodeBuffer;

#ifdef _DEBUG
        //if (m_shader != nullptr)
       // {
            //m_shader->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<UINT>(m_fileName.size()), m_fileName.c_str());
       // }
#endif
        return true;
    }

    return false;
}
