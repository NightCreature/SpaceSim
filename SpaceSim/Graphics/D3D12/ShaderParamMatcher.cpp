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
ShaderParameter::ShaderParameterData GetVariant(const std::string& name)
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
    
    return ShaderParameter::ShaderParameterData();
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
bool ShaderParamMatcher::MatchSignatureWithRefeclection(const RootParamtersInfo& rootParamInfo)
{
    bool foundRootParametersInfo = false;

    ID3D12ShaderReflection* shaderReflection;
    HRESULT hr = D3DReflect(m_shaderBlob->GetBufferPointer(), m_shaderBlob->GetBufferSize(), IID_PPV_ARGS(&shaderReflection));
    if (FAILED(hr))
    {
        MSG_TRACE_CHANNEL("ShaderParamMatcher", "Failed to get the refelection data for the the shader with error code: %d (%s)", hr, getLastErrorMessage(hr));
        return foundRootParametersInfo;
    }

    //Check and fill out rootparameter info if we have it
    ID3D12VersionedRootSignatureDeserializer* rootSignatureDeserialiser = nullptr;
    void* bufferCode = m_shaderBlob->GetBufferPointer();
    hr = D3D12CreateVersionedRootSignatureDeserializer(bufferCode, m_shaderBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignatureDeserialiser));
    if (SUCCEEDED(hr))
    {
        const D3D12_VERSIONED_ROOT_SIGNATURE_DESC* rootSignatureDescriptor = nullptr;
        hr = rootSignatureDeserialiser ? rootSignatureDeserialiser->GetRootSignatureDescAtVersion(D3D_ROOT_SIGNATURE_VERSION_1_1, &rootSignatureDescriptor) : E_FAIL;
        if (SUCCEEDED(hr))
        {
            //we have root parameters so we should save them out.
            const D3D12_ROOT_PARAMETER1* const parameters = rootSignatureDescriptor->Desc_1_1.pParameters;
            for (size_t rootParamCounter = 0; rootParamCounter < rootSignatureDescriptor->Desc_1_1.NumParameters; ++rootParamCounter)
            {
                m_rootParametersInfo.push_back(parameters[rootParamCounter]);
            }

            foundRootParametersInfo = true;
        }
    }




    D3D12_SHADER_DESC shaderDescriptor;
    hr = shaderReflection->GetDesc(&shaderDescriptor);
    if (FAILED(hr))
    {
        MSG_TRACE_CHANNEL("ShaderParamMatcher", "Failed to get shader descriptor with error code: %d (%s)", hr, getLastErrorMessage(hr));
        //return;
    }
    
    //Create the parameter setup for this shader
    if (!foundRootParametersInfo)
    {
        m_rootParametersInfo = rootParamInfo;
    }
    else
    {
        //We only need to setup the stuff for UAV, SRV and ranges once
        for (size_t rootParamIndex = 0; rootParamIndex < m_rootParametersInfo.size(); ++rootParamIndex)
        {
            D3D12_ROOT_PARAMETER1& rootParam = m_rootParametersInfo[rootParamIndex];
            //we are dealing with descriptorTable, SRV or UAV
            switch (rootParam.ParameterType)
            {
            case D3D12_ROOT_PARAMETER_TYPE_SRV:
                //This is a texture
                //MSG_TRACE("D3D12_ROOT_PARAMETER_TYPE_SRV Found");
                break;
            case D3D12_ROOT_PARAMETER_TYPE_UAV:
                //This is a bufffer
                //MSG_TRACE("D3D12_ROOT_PARAMETER_TYPE_UAV Found");
                break;
            case D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE:
                //This could be all kinds of things, in our case its just a list of textures
            {
                ShaderParameter shaderParam;
                shaderParam.m_rootParamIndex = rootParamIndex;
                if (rootParam.DescriptorTable.pDescriptorRanges[0].RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_SRV)
                {
                    TextureData data;
                    data.m_numberOfTextures = rootParam.DescriptorTable.pDescriptorRanges[0].NumDescriptors;
                    data.m_startingSlot = rootParam.DescriptorTable.pDescriptorRanges[0].BaseShaderRegister; //This indentifies the texture slot for this texture
                    shaderParam.m_data = data;
                    m_parameters.push_back(shaderParam);
                }

                //MSG_TRACE("D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE Found");
            }
                break;
            case D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS:
                //MSG_TRACE("D3D12_ROOT_PARAMETER_TYPE_32BitConstants Found");
                break;
            case D3D12_ROOT_PARAMETER_TYPE_CBV:
                //MSG_TRACE("D3D12_ROOT_PARAMETER_TYPE_CBV Found");
                break;
            default:
                break;

            }
        }
    }

    //Lets read the constant buffers and match them with the root params
    for (size_t index = 0; index < shaderDescriptor.ConstantBuffers; ++index)
    {
        ID3D12ShaderReflectionConstantBuffer* cbPtr = shaderReflection->GetConstantBufferByIndex(static_cast<UINT>(index));
        if (cbPtr != nullptr)
        {
            D3D12_SHADER_BUFFER_DESC shaderCBDesc;
            cbPtr->GetDesc(&shaderCBDesc);

            //MSG_TRACE("Adding CBV with name: %s", shaderCBDesc.Name);

            D3D12_SHADER_INPUT_BIND_DESC inputBindingDesc;
            shaderReflection->GetResourceBindingDescByName(shaderCBDesc.Name, &inputBindingDesc);

            for (size_t rootParamIndex = 0; rootParamIndex < m_rootParametersInfo.size(); ++rootParamIndex)
            {
                //We know the type has to be CBV
                D3D12_ROOT_PARAMETER1& rootParam = m_rootParametersInfo[rootParamIndex];
                if (rootParam.Descriptor.ShaderRegister == inputBindingDesc.BindPoint && rootParam.Descriptor.RegisterSpace == inputBindingDesc.Space)
                {
                    //Found a CB that matches a root paramter
                    ShaderParameter param;
                    param.m_rootParamIndex = rootParamIndex;
                    param.m_data = GetVariant(shaderCBDesc.Name);
                    m_parameters.push_back(param);

                    break;
                }
            }
        }
    }


    return foundRootParametersInfo;

}

//
/////-----------------------------------------------------------------------------
/////! @brief   
/////! @remark
/////-----------------------------------------------------------------------------
//RootParamInformation ShaderParamMatcher::HandleDescriptorTable(D3D12_ROOT_PARAMETER1 parameter, size_t rootParamIndex)
//{
//    RootParamInformation info;
//    info.m_index = rootParamIndex;
//    info.shaderRegister = parameter.Constants.ShaderRegister;
//    info.registerSpace = parameter.Constants.RegisterSpace;
//
//    return info;
//
//    for (size_t counter = 0; counter < parameter.DescriptorTable.NumDescriptorRanges; ++counter)
//    {
//        D3D12_DESCRIPTOR_RANGE1 range = parameter.DescriptorTable.pDescriptorRanges[counter];
//        if (range.BaseShaderRegister == bindingDescriptor.BindPoint && range.RegisterSpace == bindingDescriptor.Space)
//        {
//            //Found what we were looking for
//            ShaderParameter param;
//            param.m_rootParamIndex = rootParamIndex;
//            param.m_constantBuffer = GetVariant(bindingDescriptor.Name);
//            m_parameters.push_back(param);
//        }
//    }
//}
//
/////-----------------------------------------------------------------------------
/////! @brief   
/////! @remark
/////-----------------------------------------------------------------------------
//RootParamInformation ShaderParamMatcher::HandleRootConstants(D3D12_ROOT_PARAMETER1 parameter, size_t rootParamIndex)
//{
//    RootParamInformation info;
//    info.m_index = rootParamIndex;
//    info.shaderRegister = parameter.Constants.ShaderRegister;
//    info.registerSpace = parameter.Constants.RegisterSpace;
//
//    return info;
//    //if (parameter.Constants.ShaderRegister == bindingDescriptor.BindPoint && parameter.Constants.RegisterSpace == bindingDescriptor.Space)
//    //{
//    //    //Found what we were looking for
//    //    ShaderParameter param;
//    //    param.m_rootParamIndex = rootParamIndex;
//    //    param.m_constantBuffer = GetVariant(bindingDescriptor.Name);
//    //    m_parameters.push_back(param);
//    //}
//}
//
/////-----------------------------------------------------------------------------
/////! @brief   
/////! @remark
/////-----------------------------------------------------------------------------
//RootParamInformation ShaderParamMatcher::HandleCBVSRVUAV(D3D12_ROOT_PARAMETER1 parameter, size_t rootParamIndex)
//{
//
//    RootParamInformation info;
//    info.m_index = rootParamIndex;
//    info.shaderRegister = parameter.Descriptor.ShaderRegister;
//    info.registerSpace = parameter.Descriptor.RegisterSpace;
//
//    return info;
//
//    //if (parameter.Descriptor.ShaderRegister == bindingDescriptor.BindPoint && parameter.Descriptor.RegisterSpace == bindingDescriptor.Space)
//    //{
//    //    //Found what we were looking for
//    //    //Might want to check the name here for some known paramters and the data class it should represent
//    //    ShaderParameter param;
//    //    param.m_rootParamIndex = rootParamIndex;
//    //    param.m_constantBuffer = GetVariant(bindingDescriptor.Name);
//    //    m_parameters.push_back(param);
//    //}
//    ////MSG_TRACE_CHANNEL("ShaderParameterMatcher", "",);
//}