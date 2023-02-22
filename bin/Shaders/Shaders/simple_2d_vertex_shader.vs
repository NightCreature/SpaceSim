#include "BindlessBuffers.ifx"
#include "CommonStructures.ifx"
#include "rootsignatures.ifx"
#include "Samplers.ifx"
//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
ConstantBuffer<MeshResourceIndices> resourceIndices : register(b0);

//--------------------------------------------------------------------------------------
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
[RootSignature(bindlessRS)]
PS_INPUT vs_main( uint vertexID : SV_VertexID )
{
    float4 pos = float4(GetInstanceFromBufferT<float3>(resourceIndices.posBufferIndex, vertexID),0);
    ConstantBuffer<WVPData> wvpData = GetConstantBuffer<WVPData>(resourceIndices.transformIndex);
    
    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul( pos, wvpData.World );
    output.Pos = mul( output.Pos, wvpData.View );
    output.Pos = mul( output.Pos, wvpData.Projection );

    output.Tex = GetInstanceFromBufferT<float2>(resourceIndices.textureBufferIndex, vertexID);

    return output;
}