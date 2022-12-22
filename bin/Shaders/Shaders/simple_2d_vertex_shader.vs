#include "CommonConstantBuffers.ivs"
#include "rootsignatures.ifx"
//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos  : POSITION0; //xy is position zw is uv coordinates
    float2 Tex  : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
[RootSignature(bindlessRS)]
PS_INPUT vs_main( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul( input.Pos, World );
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
    //output.Pos = position;
    output.Tex = input.Tex; 
    return output;
}