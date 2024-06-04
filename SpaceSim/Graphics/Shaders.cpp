#include "Graphics/Shaders.h"

#include "Graphics/D3DDebugHelperFunctions.h"
#include "Graphics/DeviceManager.h"
#include "Core/StringOperations/StringHelperFunctions.h"
#include "Logging/LoggingMacros.h"

#include <fstream>
#include <numeric>
#include <limits>
#include "D3D12/D3D12X.h"
#include "D3D12/ShaderParamMatcher.h"
#include "D3D12/ShaderCompiler.h"
#include <dxcapi.h>
#include <D3Dcompiler.h>


const char* shaderTypeNames[] =
{
    "VS",
    "HS",
    "DS",
    "GS",
    "PS",
    "CS"
};

std::string getShaderPrefix(ShaderType type)
{
    switch (type)
    {
    case ShaderType::eVertexShader:
    {
        return "vs";
    }
    break;
    case ShaderType::eHullShader:
    {
        return "hs";
    }
    break;
    case ShaderType::eDomainShader:
    {
        return "ds";
    }
    break;
    case ShaderType::eGeometryShader:
    {
        return "gs";
    }
    break;
    case ShaderType::ePixelShader:
    {
        return "ps";
    }
    break;
    case ShaderType::eComputeShader:
    {
        return "cs";
    }
    break;
    default:
    break;
    }

    return "";
}

///-----------------------------------------------------------------------------
///! @brief   TODO enter a description
///! @remark
///-----------------------------------------------------------------------------
void getProfileName(const DeviceManager& deviceManager, ShaderType type, std::string &profileName, const std::string profileVersion = std::string())
{
    D3D_FEATURE_LEVEL featureLevel = deviceManager.getFreatureLevel();

    profileName = getShaderPrefix(type);

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
            profileName += "_6_6";//Bindless needs SM6.6 have to see if we need to chgange this to 6_6 or 6_4 and how we determine that need to include the new shader ocmpiler first and its bridge https://github.com/microsoft/DirectXShaderCompiler
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

void deserialiseSahderNode(const tinyxml2::XMLElement* element, std::string& entryPoint, std::string& fileName, ShaderType& type)
{
    for (const tinyxml2::XMLAttribute* attribute = element->FirstAttribute(); attribute != nullptr; attribute = attribute->Next())
    {
        if (strICmp(attribute->Name(), "entry_point"))
        {
            entryPoint = attribute->Value();
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


//#if defined( DEBUG ) || defined( _DEBUG )
//DWORD shaderCompilerFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR | D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG | D3DCOMPILE_ALL_RESOURCES_BOUND;
//#else
//DWORD shaderCompilerFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR | D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES;
//#endif



///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void Shader::deserialise(const tinyxml2::XMLElement* element, ShaderType defaultType)
{
    m_type = defaultType;
    deserialiseSahderNode(element, m_entryPoint, m_fileName, m_type);
    ValidateShader();
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
bool Shader::createShader(const DeviceManager& deviceManager, const ShaderCompiler& compiler)
{
    PROFILE_FUNCTION();

    std::string profileName = "";
    getProfileName(deviceManager, m_type, profileName);

    if (!compiler.CreateShader(m_fileName, convertToWideString(m_entryPoint), convertToWideString(profileName), 0, m_type == ShaderType::eVertexShader || m_type == ShaderType::eComputeShader, m_shader))
    {
        return false;
    }

    return true;
}

void Shader::ValidateShader()
{
    if (m_fileName.empty())
    {
        MSG_TRACE_CHANNEL("Shader", "Shader file name is empty");
    }
    if (m_entryPoint.empty())
    {
        MSG_TRACE_CHANNEL("Shader", "Shader entry point is empty");
    }
    if (m_type == ShaderType::Count)
    {
        MSG_TRACE_CHANNEL("Shader", "Shader type is not set");
    }
}
