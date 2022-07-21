#pragma once

#include "Graphics/D3D12/ShaderParameter.h"

#include <d3d12.h>
#include <d3d12shader.h>


struct RootParamInformation
{
	size_t m_index = 0;
	size_t shaderRegister = 0;
	size_t registerSpace = 0;
	size_t type = 0;
};


using  RootParamtersInfo = std::vector<D3D12_ROOT_PARAMETER1>;

//This should match the Root Param index with the actual data it needs, could be constant buffer setup or texture or other things
class ShaderParamMatcher
{
public:
	ShaderParamMatcher(ID3DBlob* shaderBlob);
	
	bool MatchSignatureWithRefeclection(const RootParamtersInfo& rootParamInfo);

	const ShaderParameters& GetBoundParameters() const { return m_parameters; }
		const RootParamtersInfo& GetRootParametersInfo() const { return m_rootParametersInfo; }
private:
	//RootParamInformation HandleDescriptorTable(D3D12_ROOT_PARAMETER1 parameter, size_t rootParamIndex);
	//RootParamInformation HandleRootConstants(D3D12_ROOT_PARAMETER1 parameter, size_t rootParamIndex);
	//RootParamInformation HandleCBVSRVUAV(D3D12_ROOT_PARAMETER1 parameter, size_t rootParamIndex);

	ID3DBlob* m_shaderBlob;

	ShaderParameters m_parameters;
	RootParamtersInfo m_rootParametersInfo;
};