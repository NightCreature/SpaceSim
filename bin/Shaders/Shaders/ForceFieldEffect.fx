#include "CommonConstantBuffers.ivs"
#include "CommonConstantBuffers.ips"
#include "rootsignatures.ifx"
//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------

Texture2D<float4> shaderTextures: register(t8);
SamplerState SampleType : register (s0);

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos  : POSITION0;
    float3 Nor  : NORMAL0;
    float2 Tex1 : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
    float2 Tex2 : TEXCOORD1;
};

// function for getting the checker pattern
float4 checker(float2 uv) 
{
    float checkSize = 4;
    float2 t = abs(  floor(uv) );
    int2 s = floor(t * 2);
    if (floor(s[0] + s[1]) % 2 == 0) 
    {
        return float4(0, 0, 0, 1);  
    } 
    else
    {
        return float4(1, 1, 1, 1);  
    }
}

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
[RootSignature(forceFieldRS)]
PS_INPUT vs_main( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul( input.Pos, World );
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
    output.Tex = input.Tex1; 
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 ps_main( PS_INPUT input) : SV_Target
{
    float4 color = ambient + shaderTextures.Sample(SampleType, input.Tex).a * diffuse + emissive;
    color.a = shaderTextures.Sample(SampleType, input.Tex).a;
    return saturate(color);
}
