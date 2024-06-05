#include "Graphics/RenderGraph/Pass.h"

#include "Core/Types/TypeHelpers.h"

#include "Graphics/RenderGraph/ResourceDescriptor.h"
#include "Graphics/RenderGraph/RenderGraph.h"


void UseRenderGraph()
{
	TextureResourceProprties colorPassTexture;

	std::array<size_t, 3>textureSize = { 2540, 1440, 0 };
	colorPassTexture.m_dimensions.Set(textureSize);
	colorPassTexture.m_format = DXGI_FORMAT_B8G8R8A8_UNORM;

	TextureResourceProprties shadowMapProperties;
	shadowMapProperties.m_dimensions.Set(textureSize);
	shadowMapProperties.m_format = DXGI_FORMAT_R32_FLOAT;

	auto ColorPassExecuteFunction = [](const PassResources& resources) { UNUSEDPARAM(resources); };
	RenderGraph rg;
	auto& colorPass = rg.AddPass("ColorOuputPass", ColorPassExecuteFunction);
	RenderResourceProperties colorPassOuput("ColorOuput");
	colorPassOuput.SetData(colorPassTexture);
	RenderResourceProperties shadowMapInput("ShadowMapInput");
	shadowMapInput.SetData(shadowMapProperties);

	
	colorPass.AddOuput(colorPassOuput);
	colorPass.AddInput(shadowMapInput);

	auto& shadowMapPass = rg.AddPass("ShadowMap", ColorPassExecuteFunction);
	RenderResourceProperties showMapOutput("ShadowMapOuput");
	
	shadowMapPass.AddOuput(showMapOutput);
	showMapOutput.SetData(shadowMapProperties);
}