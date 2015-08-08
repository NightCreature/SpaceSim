//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer WVPConstants : register(b0)
{
    float4x4 World;
    float4x4 View;
    float4x4 Projection;
};

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos  : POSITION0; //xy is position zw is uv coordinates
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT vs_main( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
    float4 position = float4(input.Pos.xy, 0, 0);
    output.Pos = mul( position, World );
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
    output.Tex = input.Pos.zw; 
    return output;
}