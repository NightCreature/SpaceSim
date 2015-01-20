//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer WVPConstants : register( b0 )
{
    matrix World;
    matrix View;
    matrix Projection;
};

cbuffer MaterialConstants : register(b0)
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 emissive;
    float shininess;
};

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos  : POSITION;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT vs_main( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul( input.Pos, World );
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 ps_main( PS_INPUT input) : SV_Target
{
    float4 color = saturate(diffuse + emissive);
    color.a = 0.5;
    return color;
}