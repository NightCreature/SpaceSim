#pragma once

#include "Graphics/D3D12/ShaderParameter.h"

#include <d3d12.h>
#include <d3d12shader.h>

//This should match the Root Param index with the actual data it needs, could be constant buffer setup or texture or other things
class ShaderParamMatcher
{
public:
	ShaderParamMatcher(ID3DBlob* shaderBlob);
	
	void MatchSignatureWithRefeclection();

	const ShaderParameters& GetBoundParameters() const { return m_parameters; }

private:
	void HandleDescriptorTable(D3D12_ROOT_PARAMETER1 parameter, D3D12_SHADER_INPUT_BIND_DESC bindingDescriptor, size_t rootParamIndex);
	void HandleRootConstants(D3D12_ROOT_PARAMETER1 parameter, D3D12_SHADER_INPUT_BIND_DESC bindingDescriptor, size_t rootParamIndex);
	void HandleCBVSRVUAV(D3D12_ROOT_PARAMETER1 parameter, D3D12_SHADER_INPUT_BIND_DESC bindingDescriptor, size_t rootParamIndex);

	ID3DBlob* m_shaderBlob;

	ShaderParameters m_parameters;
};