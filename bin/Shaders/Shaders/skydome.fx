#include "CommonConstantBuffers.ivs"

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer MaterialConstants : register(b0)
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 emissive;
    float shininess;
};

Texture2D<float4> shaderTextures;
SamplerState SampleType
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos  : POSITION0;
    float3 Nor  : NORMAL0;
    float2 Tex1 : TEXCOORD0;
    //float2 Tex2 : TEXCOORD1;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Nor : NORMAL0;
    float2 Tex : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT vs_main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul(input.Pos, World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    output.Nor = input.Nor;
    output.Tex = input.Tex1;
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 ps_main(PS_INPUT input) : SV_Target
{
    return saturate( shaderTextures.Sample(SampleType, input.Tex) );
}