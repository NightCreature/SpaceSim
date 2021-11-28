#include "Graphics/D3D12/ShaderParamMatcher.h"

#include "Core/StringOperations/StringHelperFunctions.h"
#include "Graphics/Effect.h"

#include <D3Dcompiler.h>
#include <d3d12.h>

//void OutputRange(std::stringstream& out, D3D12_DESCRIPTOR_RANGE1 range)
//{
//	out << "Number of Descriptors: " << range.NumDescriptors;
//	out << " ,Range Type: " << range.RangeType;
//	out << " ,Base Shader Register: " << range.BaseShaderRegister;
//	out << " ,RegisterSpace: " << range.RegisterSpace;
//	out << " ,Flags: " << range.Flags;
//	out << " ,Offset From Table Start: " << range.OffsetInDescriptorsFromTableStart;
//	out << "\n";
//}
//
//
/////-----------------------------------------------------------------------------
/////! @brief   
/////! @remark
/////-----------------------------------------------------------------------------
//void OutputDescriptorTable(std::stringstream& out, D3D12_ROOT_DESCRIPTOR_TABLE1 descriptorTable)
//{
//	out << "Descriptor Table:\n";
//	for (size_t counter = 0; counter < descriptorTable.NumDescriptorRanges; ++counter)
//	{
//		OutputRange(out, descriptorTable.pDescriptorRanges[counter]);
//	}
//}
//
/////-----------------------------------------------------------------------------
/////! @brief   
/////! @remark
/////-----------------------------------------------------------------------------
//void OuputConstants(std::stringstream& out, D3D12_ROOT_CONSTANTS constants)
//{
//	out << "Constants:\nShaderRegister: " << constants.ShaderRegister << ", RegisterSpace: " << constants.RegisterSpace << ", Number of Values: " << constants.Num32BitValues << "\n";
//}
//
/////-----------------------------------------------------------------------------
/////! @brief   
/////! @remark
/////-----------------------------------------------------------------------------
//void OuputDescriptor(std::stringstream& out, D3D12_ROOT_DESCRIPTOR1 descriptor)
//{
//	out << "Descriptor:\nFlags:";
//	switch (descriptor.Flags)
//	{
//	case D3D12_ROOT_DESCRIPTOR_FLAG_NONE: out << "None"; break;
//	case D3D12_ROOT_DESCRIPTOR_FLAG_DATA_VOLATILE: out << "Data Volatile"; break;
//	case D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE: out << "Static with set at execute"; break;
//	case D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC: out << "Static"; break;
//	}
//
//	out << ", RegisterSpace: " << descriptor.RegisterSpace << ", ShaderRegister : " << descriptor.ShaderRegister;
//	out << "\n";
//}
//
//void OutputParameter(std::stringstream& out, const D3D12_ROOT_PARAMETER1& parameter)
//{
//
//	switch (parameter.ParameterType)
//	{
//
//	case D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE:
//		out << "ParamType: Descriptor Table\n";
//		OutputDescriptorTable(out, parameter.DescriptorTable);
//		break;
//	case D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS:
//		out << "ParamType: 32 Bit Constants\n";
//		OuputConstants(out, parameter.Constants);
//		break;
//	case D3D12_ROOT_PARAMETER_TYPE_CBV:
//		out << "ParamType: CBV\n";
//		OuputDescriptor(out, parameter.Descriptor);
//		break;
//	case D3D12_ROOT_PARAMETER_TYPE_SRV:
//		out << "ParamType: SRV\n";
//
//		OuputDescriptor(out, parameter.Descriptor);
//		break;
//	case D3D12_ROOT_PARAMETER_TYPE_UAV:
//		out << "ParamType: UAV\n";
//		OuputDescriptor(out, parameter.Descriptor);
//		break;
//	default:
//		break;
//	}
//}
//
//
////Lets see what these layouts look like
//MSG_TRACE_CHANNEL("Shader", "Rootsignature Layout for shader: %s ", m_fileName.c_str());
//ID3D12VersionedRootSignatureDeserializer* deserialiser = nullptr;
//hr = D3D12CreateVersionedRootSignatureDeserializer(m_shaderBlob->GetBufferPointer(), m_shaderBlob->GetBufferSize(), IID_PPV_ARGS(&deserialiser));
//if (FAILED(hr))
//{
//	MSG_TRACE_CHANNEL("Shader_ERROR", "Failed to deserialse the root signature with error code: 0x%x(%s)", hr, getLastErrorMessage(hr));
//}
//else
//{
//	const D3D12_VERSIONED_ROOT_SIGNATURE_DESC* desc;
//	hr = deserialiser->GetRootSignatureDescAtVersion(D3D_ROOT_SIGNATURE_VERSION_1_1, &desc);
//	if (SUCCEEDED(hr))
//	{
//		for (size_t counter = 0; counter < desc->Desc_1_1.NumParameters; ++counter)
//		{
//			std::stringstream str;
//			str << "Root Parameter: " << counter << "\n";
//			OutputParameter(str, desc->Desc_1_1.pParameters[counter]);
//			MSG_TRACE_CHANNEL("Shader", "%s ", str.str().c_str());
//		}
//	}
//}
//
//MSG_TRACE_CHANNEL("Shader", "Shader Reflection: %s ", m_fileName.c_str());
//ID3D12ShaderReflection* reflection;
//hr = D3DReflect(m_shaderBlob->GetBufferPointer(), m_shaderBlob->GetBufferSize(), IID_PPV_ARGS(&reflection));
//if (SUCCEEDED(hr))
//{
//	D3D12_SHADER_DESC descriptor;
//	hr = reflection->GetDesc(&descriptor);
//	if (SUCCEEDED(hr))
//	{
//		MSG_TRACE_CHANNEL("Shader", "Num Constantbuffers: %d, Num BoundResources: %d", descriptor.ConstantBuffers, descriptor.BoundResources);
//		MSG_TRACE_CHANNEL("Shader", "Constant Buffers");
//		for (size_t counter = 0; counter < descriptor.ConstantBuffers; ++counter)
//		{
//			auto* constantBufferReflection = reflection->GetConstantBufferByIndex(counter);
//			D3D12_SHADER_BUFFER_DESC bufferDesc;
//			hr = constantBufferReflection->GetDesc(&bufferDesc);
//			MSG_TRACE_CHANNEL("Shader", "Name: %s", bufferDesc.Name);
//			MSG_TRACE_CHANNEL("Shader", "Num Variables: %d", bufferDesc.Variables);
//			for (size_t varCount = 0; varCount < bufferDesc.Variables; ++varCount)
//			{
//				D3D12_SHADER_VARIABLE_DESC varDesc;
//				auto* varReflect = constantBufferReflection->GetVariableByIndex(varCount);
//				hr = varReflect->GetDesc(&varDesc);
//				MSG_TRACE_CHANNEL("Shader", "Name: %s", varDesc.Name);
//				MSG_TRACE_CHANNEL("Shader", "Num Variables: %d", varDesc.Size);
//			}
//		}
//
//		MSG_TRACE_CHANNEL("Shader", "Bound Resources");
//		for (size_t counter = 0; counter < descriptor.BoundResources; ++counter)
//		{
//			D3D12_SHADER_INPUT_BIND_DESC inputBind;
//			reflection->GetResourceBindingDesc(counter, &inputBind);
//			MSG_TRACE_CHANNEL("Shader", "Name: %s", inputBind.Name);
//			MSG_TRACE_CHANNEL("Shader", "Type: %d", inputBind.Type);
//			MSG_TRACE_CHANNEL("Shader", "Space: %d", inputBind.Space);
//			MSG_TRACE_CHANNEL("Shader", "Register: %d", inputBind.BindPoint);
//		}
//	}
//	else
//	{
//		MSG_TRACE_CHANNEL("Shader_ERROR", "Failed to reflect on the sahder with error code: 0x%x(%s)", hr, getLastErrorMessage(hr));
//	}
//
//}
//else
//{
//	MSG_TRACE_CHANNEL("Shader_ERROR", "Failed to reflect on the sahder with error code: 0x%x(%s)", hr, getLastErrorMessage(hr));
//}

//This needs to be done differently in the future
std::variant<ConstantBuffer<WVPBufferContent>, ConstantBuffer<MaterialContent>, ConstantBuffer<PerFrameConstants>> GetVariant(const std::string& name)
{
    if (name == "WVPConstants" || name == "ShadowConstants")
    {
        return ConstantBuffer<WVPBufferContent>();
    }
    else if (name == "LightParameters")
    { 
        return ConstantBuffer<PerFrameConstants>();
    }
    else if (name == "MaterialConstants")
    { 
        return ConstantBuffer<MaterialContent>();
    }
    
    return std::variant<ConstantBuffer<WVPBufferContent>, ConstantBuffer<MaterialContent>, ConstantBuffer<PerFrameConstants>>();
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
ShaderParamMatcher::ShaderParamMatcher(ID3DBlob* shaderBlob) : m_shaderBlob(shaderBlob)
{
    //m_factory.RegisterDataTypeForName<WVPBufferContent>("WVPConstants");
    //m_factory.RegisterDataTypeForName<WVPBufferContent>("ShadowConstants");
    //m_factory.RegisterDataTypeForName<PerFrameConstants>("LightParameters");
    //m_factory.RegisterDataTypeForName<MaterialContent>("MaterialConstants");
    

    //auto object = constructTypeFromPack<2, WVPBufferContent, PerFrameConstants, MaterialContent>();
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void ShaderParamMatcher::MatchSignatureWithRefeclection()
{

    ID3D12ShaderReflection* shaderReflection;
    HRESULT hr = D3DReflect(m_shaderBlob->GetBufferPointer(), m_shaderBlob->GetBufferSize(), IID_PPV_ARGS(&shaderReflection));
    if (FAILED(hr))
    {
        MSG_TRACE_CHANNEL("ShaderParamMatcher", "Failed to get the refelection data for the the shader with error code: %d (%s)", hr, getLastErrorMessage(hr));
        return;
    }

    ID3D12VersionedRootSignatureDeserializer* rootSignatureDeserialiser = nullptr;
    void* bufferCode = m_shaderBlob->GetBufferPointer();
    hr = D3D12CreateVersionedRootSignatureDeserializer(bufferCode, m_shaderBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignatureDeserialiser));
    if (FAILED(hr))
    {
        MSG_TRACE_CHANNEL("ShaderParamMatcher", "Failed to deserialise the root signature with error code: %d (%s)", hr, getLastErrorMessage(hr));
        return;
    }

    const D3D12_VERSIONED_ROOT_SIGNATURE_DESC* rootSignatureDescriptor;
    hr = rootSignatureDeserialiser->GetRootSignatureDescAtVersion(D3D_ROOT_SIGNATURE_VERSION_1_1, &rootSignatureDescriptor);
    if (FAILED(hr))
    {
        MSG_TRACE_CHANNEL("ShaderParamMatcher", "Failed to get root signature descriptor with error code: %d (%s)", hr, getLastErrorMessage(hr));
        return;
    }


    D3D12_SHADER_DESC shaderDescriptor;
    hr = shaderReflection->GetDesc(&shaderDescriptor);
    if (FAILED(hr))
    {
        MSG_TRACE_CHANNEL("ShaderParamMatcher", "Failed to get shader descriptor with error code: %d (%s)", hr, getLastErrorMessage(hr));
        return;
    }
    //Got everything we need here now lets match the bound resources from the reflection data with the root params
    for (size_t boundResourceCounter = 0; boundResourceCounter < shaderDescriptor.BoundResources; ++boundResourceCounter)
    {
        D3D12_SHADER_INPUT_BIND_DESC inputBindingDescriptor;
        hr = shaderReflection->GetResourceBindingDesc(static_cast<UINT>(boundResourceCounter), &inputBindingDescriptor);
        if (SUCCEEDED(hr))
        {
            const D3D12_ROOT_PARAMETER1* const parameters = rootSignatureDescriptor->Desc_1_1.pParameters;
            for (size_t rootParamCounter = 0; rootParamCounter < rootSignatureDescriptor->Desc_1_1.NumParameters; ++rootParamCounter)
            {
                switch (parameters[rootParamCounter].ParameterType)
                {
                case D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE:
                    HandleDescriptorTable(parameters[rootParamCounter], inputBindingDescriptor, rootParamCounter); //Special
                    break;
                case D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS:
                    HandleRootConstants(parameters[rootParamCounter], inputBindingDescriptor, rootParamCounter); //special
                    break;
                case D3D12_ROOT_PARAMETER_TYPE_CBV:
                case D3D12_ROOT_PARAMETER_TYPE_SRV:
                case D3D12_ROOT_PARAMETER_TYPE_UAV:
                    HandleCBVSRVUAV(parameters[rootParamCounter], inputBindingDescriptor, rootParamCounter);
                    break;
                default:
                    break;
                }
            }
        }
    }
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void ShaderParamMatcher::HandleDescriptorTable(D3D12_ROOT_PARAMETER1 parameter, D3D12_SHADER_INPUT_BIND_DESC bindingDescriptor, size_t rootParamIndex)
{
    for (size_t counter = 0; counter < parameter.DescriptorTable.NumDescriptorRanges; ++counter)
    {
        D3D12_DESCRIPTOR_RANGE1 range = parameter.DescriptorTable.pDescriptorRanges[counter];
        if (range.BaseShaderRegister == bindingDescriptor.BindPoint && range.RegisterSpace == bindingDescriptor.Space)
        {
            //Found what we were looking for
            ShaderParameter param;
            param.m_rootParamIndex = rootParamIndex;
            param.m_constantBuffer = GetVariant(bindingDescriptor.Name);
            m_parameters.push_back(param);
        }
    }
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void ShaderParamMatcher::HandleRootConstants(D3D12_ROOT_PARAMETER1 parameter, D3D12_SHADER_INPUT_BIND_DESC bindingDescriptor, size_t rootParamIndex)
{
    if (parameter.Constants.ShaderRegister == bindingDescriptor.BindPoint && parameter.Constants.RegisterSpace == bindingDescriptor.Space)
    {
        //Found what we were looking for
        ShaderParameter param;
        param.m_rootParamIndex = rootParamIndex;
        param.m_constantBuffer = GetVariant(bindingDescriptor.Name);
        m_parameters.push_back(param);
    }
}

///-----------------------------------------------------------------------------
///! @brief   
///! @remark
///-----------------------------------------------------------------------------
void ShaderParamMatcher::HandleCBVSRVUAV(D3D12_ROOT_PARAMETER1 parameter, D3D12_SHADER_INPUT_BIND_DESC bindingDescriptor, size_t rootParamIndex)
{
    if (parameter.Descriptor.ShaderRegister == bindingDescriptor.BindPoint && parameter.Descriptor.RegisterSpace == bindingDescriptor.Space)
    {
        //Found what we were looking for
        //Might want to check the name here for some known paramters and the data class it should represent
        ShaderParameter param;
        param.m_rootParamIndex = rootParamIndex;
        param.m_constantBuffer = GetVariant(bindingDescriptor.Name);
        m_parameters.push_back(param);
    }
}