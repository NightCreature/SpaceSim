#define LIGHTHING
#define SHADOW
#include "CommonConstantBuffers.ivs"

///--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos  : POSITION0;
    float3 Nor  : NORMAL0;
    float3 Tan  : TANGENT0;
    float2 Tex1 : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Nor : NORMAL0;
    float3 Tan : TANGENT0;
    float3 BiN : BINORMAL0;
    float2 Tex : TEXCOORD0;
    float3 WorldPos : TEXCOORD1;
    float4 LightToPixel : TEXCOORD2;
};

///--------------------------------------------------------------------------------------
// Vertex Shader
///--------------------------------------------------------------------------------------
PS_INPUT vs_main( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul( input.Pos, World );
    output.WorldPos = output.Pos.xyz;
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );

    float3 biNormal = cross(input.Nor, input.Tan);

    output.Nor = input.Nor;
    output.Tan = input.Tan;
    output.BiN = biNormal;

    float4 pos = float4(input.Pos.xyz, 1.0);
    output.LightToPixel = mul( pos, World );
    output.LightToPixel = mul( output.LightToPixel, ShadowView );
    output.LightToPixel = mul( output.LightToPixel, ShadowProjection );

    //output.inpos = input.Tex1;
    //output.Pos = input.Pos;
    output.Tex = input.Tex1; 
    return output;
}