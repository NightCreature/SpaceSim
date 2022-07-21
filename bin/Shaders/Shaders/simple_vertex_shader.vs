//#define LIGHTHING
#define SHADOW
#include "CommonConstantBuffers.ivs"
//#include "rootsignatures.ifx"

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

//#define simpleEffectRS\
//    "RootFlags( ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT ),"\
//    "CBV(b0, space=0),"\
//    "CBV(b1, space=0),"\
//    "CBV(b2, space=0),"\
//    "CBV(b0, space=1),"\
//    "CBV(b1, space=1),"\
//    "DescriptorTable( SRV(t8) ),"\
//    "DescriptorTable( SRV(t32) ),"\
//    "DescriptorTable( SRV(t33) ),"\
//    "StaticSampler(s0, "\
//               "addressU = TEXTURE_ADDRESS_WRAP, "\
//               "addressV = TEXTURE_ADDRESS_WRAP, "\
//               "filter = FILTER_ANISOTROPIC ),"\
//    "StaticSampler(s1, "\
//               "addressU = TEXTURE_ADDRESS_WRAP, "\
//               "addressV = TEXTURE_ADDRESS_WRAP, "\
//               "filter = FILTER_ANISOTROPIC )"

//"CBV(b1, space=0, visibility = SHADER_VISIBILITY_ALL),"\

#define simpleEffectRS\
    "RootFlags( ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT ),"\
    "CBV(b0, space=0, visibility = SHADER_VISIBILITY_ALL),"\
    "CBV(b2, space=0, visibility = SHADER_VISIBILITY_ALL),"\
    "CBV(b0, space=1, visibility = SHADER_VISIBILITY_ALL),"\
    "CBV(b1, space=1, visibility = SHADER_VISIBILITY_ALL),"\
    "DescriptorTable( SRV(t8), visibility = SHADER_VISIBILITY_All )," \
    "DescriptorTable( SRV(t32), visibility = SHADER_VISIBILITY_ALL )," \
    "DescriptorTable( SRV(t33), visibility = SHADER_VISIBILITY_ALL )," \
    "StaticSampler(s0, " \
               "addressU = TEXTURE_ADDRESS_WRAP, " \
               "addressV = TEXTURE_ADDRESS_WRAP, " \
               "filter = FILTER_ANISOTROPIC )," \
    "StaticSampler(s1, " \
               "addressU = TEXTURE_ADDRESS_WRAP, " \
               "addressV = TEXTURE_ADDRESS_WRAP, " \
               "filter = FILTER_ANISOTROPIC )"

///--------------------------------------------------------------------------------------
// Vertex Shader
///--------------------------------------------------------------------------------------
[RootSignature(simpleEffectRS)]
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