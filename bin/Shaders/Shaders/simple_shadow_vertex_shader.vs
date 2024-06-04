#include "BindlessBuffers.ifx"
#include "Rootsignatures.ifx"

ConstantBuffer<MeshResourceIndices> resourceIndices : register(b0);

//--------------------------------------------------------------------------------------
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Nor : NORMAL0;
    float3 Tan : TANGENT0;
    float3 BiN : BINORMAL0;
    float2 Tex : TEXCOORD0;
    float3 WorldPos : TEXCOORD1;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
[RootSignature(bindlessRS)]
PS_INPUT vs_main( uint id:SV_VERTEXID )
{
    float4 pos = float4(GetInstanceFromBufferT<float3>(resourceIndices.posBufferIndex, id),1);
    ConstantBuffer<WVPData> wvpData = GetConstantBuffer<WVPData>(resourceIndices.transformIndex);
    ConstantBuffer<WVPData> perScene = GetConstantBuffer<WVPData>(resourceIndices.sceneTransformIndex);

    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul( pos, wvpData.World );
    output.WorldPos = output.Pos.xyz;
    output.Pos = mul( output.Pos, perScene.View );
    output.Pos = mul( output.Pos, perScene.Projection );

    output.Nor = GetInstanceFromBufferT<float3>(resourceIndices.normalBufferIndex, id);
    output.Tan = GetInstanceFromBufferT<float3>(resourceIndices.tangentBufferIndex, id);
    output.BiN = cross(output.Nor, output.Tan);

    //output.inpos = input.Tex1;
    //output.Pos = input.Pos;
    output.Tex = GetInstanceFromBufferT<float2>(resourceIndices.textureBufferIndex, id);
    return output;
}