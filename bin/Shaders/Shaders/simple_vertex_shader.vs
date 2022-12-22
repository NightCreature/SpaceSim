//#define LIGHTHING
#define SHADOW
//#include "CommonConstantBuffers.ivs"
#include "rootsignatures.ifx"
#include "BindlessBuffers.ifx"

ConstantBuffer<MeshResourceIndices> resourceIndices : register(b0);

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

[RootSignature(bindlessRS)]
PS_INPUT vs_main( uint vertexID : SV_VertexID )
{
    float4 pos = float4(GetInstanceFromBufferT<float3>(resourceIndices.posBufferIndex, vertexID),0);
    WVPData wvpData = GetInstanceFromBuffer<WVPData>(resourceIndices.transformIndex);

    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul( pos, wvpData.World );
    output.WorldPos = output.Pos.xyz;
    output.Pos = mul( output.Pos, wvpData.View );
    output.Pos = mul( output.Pos, wvpData.Projection );

    output.Nor = GetInstanceFromBufferT<float3>(resourceIndices.normalBufferIndex, vertexID);
    output.Tan = GetInstanceFromBufferT<float3>(resourceIndices.tangentBufferIndex, vertexID);
    output.BiN = cross(output.Nor, output.Tan);

    float4 shadowPos = float4(pos.xyz, 1.0);
    WVPData shadowWVP = GetInstanceFromBuffer<WVPData>(resourceIndices.ShadowProjection);
    output.LightToPixel = mul( shadowPos, wvpData.World );
    output.LightToPixel = mul( output.LightToPixel, shadowWVP.View );
    output.LightToPixel = mul( output.LightToPixel, shadowWVP.Projection );

    //output.inpos = input.Tex1;
    //output.Pos = input.Pos;
    output.Tex = GetInstanceFromBufferT<float2>(resourceIndices.textureBufferIndex, vertexID ); 
    return output;
}