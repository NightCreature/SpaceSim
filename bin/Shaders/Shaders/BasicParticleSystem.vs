//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer WVPConstants : register(b0)
{
    float4x4 World;
    float4x4 View;
    float4x4 Projection;
};

cbuffer InverseView : register(b1)
{
	float4x4 InverseView;
}

struct ParticleData
{
	float4 position;
	float4 color;
	float size;
};

StructuredBuffer<ParticleData> data : register(t0);

struct VSOutput
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
};

VSOutput vs_main(uint id:SV_VERTEXID)
{
	uint particleIndex = id / 4;
	uint vertexInQuad = id % 4;

	float3 position;
	position.x = (vertexInQuad % 2) ? 1.0 : -1.0;
	position.y = (vertexInQuad & 2) ? -1.0 : 1.0;
	position.z = 0.0;
	position.xy *= 0.5;//data[particleIndex].size.x;

	position = mul(position, (float3x3)InverseView) + data[particleIndex].position.xyz; //transition from screen space to world space and add world space position
	VSOutput output = (VSOutput)0;
    output.Pos = mul( float4(position, 1.0), World );
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );

    output.Tex.x = (vertexInQuad % 2) ? 1.0 : 0.0;
    output.Tex.y = (vertexInQuad & 2) ? 1.0 : 0.0;

    return output;
}