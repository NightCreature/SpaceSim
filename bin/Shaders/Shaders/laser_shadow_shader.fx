#include "BindlessBuffers.ifx"
#include "CommonStructures.ifx"
#include "rootsignatures.ifx"
#include "Samplers.ifx"
//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
ConstantBuffer<MeshResourceIndices> resourceIndices : register(b0);

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
[RootSignature(bindlessRS)]
PS_INPUT vs_main( uint vertexID : SV_VertexID )
{
    float4 pos = float4(GetInstanceFromBufferT<float3>(resourceIndices.posBufferIndex, vertexID),0);
    ConstantBuffer<WVPData> wvpData = GetConstantBuffer<WVPData>(resourceIndices.transformIndex);
    ConstantBuffer<WVPData> perScene = GetConstantBuffer<WVPData>(resourceIndices.sceneTransformIndex);

    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul( pos, wvpData.World );
    output.Pos = mul( output.Pos, perScene.View );
    output.Pos = mul( output.Pos, perScene.Projection );

    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 ps_main( PS_INPUT input) : SV_Target
{
    float4 color = float4(input.Pos.z, 1.0, 1.0, 1.0);
    return color; 
}