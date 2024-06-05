#include "BindlessBuffers.ifx"
#include "Rootsignatures.ifx"

ConstantBuffer<MeshResourceIndices> resourceIndices : register(b0);

//--------------------------------------------------------------------------------------
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
[RootSignature(bindlessRS)]
PS_INPUT vs_main( uint id:SV_VERTEXID )
{
    float3 posBuffer = GetInstanceFromBufferT<float3>(resourceIndices.posBufferIndex, id);
    ConstantBuffer<WVPData> wvpData = GetConstantBuffer<WVPData>(resourceIndices.transformIndex);
    ConstantBuffer<WVPData> perScene = GetConstantBuffer<WVPData>(resourceIndices.sceneTransformIndex);
    ConstantBuffer<MaterialConstants> material = GetConstantBuffer<MaterialConstants>(resourceIndices.materialIndex);

    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul( float4(posBuffer,1.0f), wvpData.World );
    output.Pos = mul( output.Pos, perScene.View );
    output.Pos = mul( output.Pos, perScene.Projection );

    output.Color = float4(1,1,1,1);
    return output;
}
