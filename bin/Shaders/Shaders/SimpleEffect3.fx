//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer WVPConstants : register(cb0)
{
    matrix World;
    matrix View;
    matrix Projection;
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
    float4 Pos  : POSITION;
    float3 Nor  : NORMAL;
    float2 Tex1 : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
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
PS_INPUT vs_main( VS_INPUT input )
{
    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul( input.Pos, World );
    output.Pos = mul( output.Pos, View );
    output.Pos = mul( output.Pos, Projection );
    //output.inpos = input.Tex1;
    //output.Pos = input.Pos;
    output.Tex = input.Tex1; 
    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 ps_main( PS_INPUT input) : SV_Target
{
    return float4(1,1,1,1);
}
