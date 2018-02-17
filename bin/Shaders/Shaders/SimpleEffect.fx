#include "CommonConstantBuffers.ivs"

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer GSViewMatrices : register(b1)
{
    float4x4 viewMatrix[6];
}

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
//Stage Structures
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos  : POSITION0;
    float3 Nor  : NORMAL0;
    float2 Tex1 : TEXCOORD0;
    //float2 Tex2 : TEXCOORD1;
};

struct GS_CUBEMAP_IN
{
    float4 Pos		: SV_POSITION;    // World position
    float3 Normal	: NORMAL;
    float2 Tex		: TEXCOORD0;         // Texture coord
};


struct PS_CUBEMAP_IN
{
    float4 Pos : SV_POSITION;
    float3 Nor : NORMAL0;
    float2 Tex : TEXCOORD0;
    float3 WorldPos : TEXCOORD1;
    uint RTIndex : SV_RenderTargetArrayIndex;
};

//--------------------------------------------------------------------------------------
// Shader Code
//--------------------------------------------------------------------------------------
GS_CUBEMAP_IN vs_main(VS_INPUT input)
{
    GS_CUBEMAP_IN output = (GS_CUBEMAP_IN)0;
    output.Pos = mul(input.Pos, World);
    output.Normal = input.Nor;
    output.Tex = input.Tex1;
    return output;
}

[maxvertexcount(18)]
void gs_main(triangle GS_CUBEMAP_IN input[3], inout TriangleStream<PS_CUBEMAP_IN> CubeMapStream)
{
    for (int f = 0; f < 6; ++f)
    {
        // Compute screen coordinates
        PS_CUBEMAP_IN output;
        output.RTIndex = f;
        for (int v = 0; v < 3; v++)
        {
            output.Pos = mul(input[v].Pos, viewMatrix[f]);
            output.Pos = mul(output.Pos, Projection);
            output.Nor = input[v].Normal;
            output.Tex = input[v].Tex;
            output.WorldPos = input[v].Pos.xyz;
            CubeMapStream.Append(output);
        }
        CubeMapStream.RestartStrip();
    }
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 ps_main( PS_CUBEMAP_IN input) : SV_Target
{
    float3 vertexPos = input.WorldPos;
    float4 color;
    float4 texColor = shaderTextures.Sample(SampleType, input.Tex);
    float3 nnormal = normalize(input.Nor);
    float3 nviewdirection = normalize(camera_position - vertexPos);
    color = float4(ambient.rgb, 1.0) * texColor + float4(emissive.rgb, 0.0);
    for (int counter = 0; counter < 3; ++counter)
    {
        float3 lightDir = m_lights[counter].m_position.xyz - vertexPos;
        float3 distance = dot(lightDir, lightDir);
        if (distance <= (m_lights[counter].m_range.x * m_lights[counter].m_range.x))
        {
            lightDir = normalize(lightDir);
            float ndotl = dot(nnormal, -lightDir);

            float3 reflection = normalize(((2.0 * nnormal) * ndotl) - lightDir);
            float rdotv = pow(saturate(dot(reflection, nviewdirection)), shininess);

            if (ndotl > 0.0)
            {
                color = color + (m_lights[counter].m_diffuse * ndotl * (texColor * float4(diffuse.rgb, 1.0)));// + rdotv;
            }
        }
    }
    //color.rgb = nnormal;
    //color = texColor;
	color = float4(1,0,0,1);
    return saturate(color);
}