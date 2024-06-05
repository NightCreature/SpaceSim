#include "CommonConstantBuffers.ivs"
#include "Rootsignatures.ifx"

#include "BindlessBuffers.ifx"
//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------

ConstantBuffer<ParticleData> resourceIndices : register(b0);

struct VSOutput
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
	uint textureIndex : TEXCOORD1;
};

[RootSignature(bindlessRS)]
VSOutput vs_main(uint id:SV_VERTEXID)
{
	uint particleIndex = id / 4;
	uint vertexInQuad = id % 4;


	StructuredBuffer<float3> posData = GetBufferT<float3>(resourceIndices.positionIndex);
	StructuredBuffer<float4> colorData = GetBufferT<float4>(resourceIndices.colorIndex);
	StructuredBuffer<float4> sizeData = GetBufferT<float4>(resourceIndices.sizeIndex);
	WVPData wvpData = GetInstanceFromBuffer<WVPData>(resourceIndices.transformIndex);

	float3 position;
	position.x = (vertexInQuad % 2) ? 1.0 : -1.0;
	position.y = (vertexInQuad & 2) ? -1.0 : 1.0;
	position.z = 0.0;
	position.xy *= 0.5;//sizeData[particleIndex].size.x;

	WVPData inverseView = GetInstanceFromBuffer<WVPData>(resourceIndices.transformIndex);
	position = mul(position, (float3x3)inverseView.View); //Game needs to store inverse view in this view object
	position += posData[particleIndex]; //transition from screen space to world space and add world space position
	VSOutput output = (VSOutput)0;
    output.Pos = mul( float4(position, 1.0), wvpData.World );
    output.Pos = mul( output.Pos, wvpData.View );
    output.Pos = mul( output.Pos, wvpData.Projection );

    output.Tex.x = (vertexInQuad % 2) ? 1.0 : 0.0;
    output.Tex.y = (vertexInQuad & 2) ? 1.0 : 0.0;

	output.textureIndex = resourceIndices.textureIndex;

    return output;
}