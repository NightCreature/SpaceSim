#include "BindlessBuffers.ifx"
#include "Rootsignatures.ifx"

ConstantBuffer<MeshResourceIndices> renderIndices : register(b0);

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
    float3 posBuffer = GetInstanceFromBufferT<float3>(renderIndices.posBufferIndex, id);
    ConstantBuffer<WVPData> wvpData = GetConstantBuffer<WVPData>(renderIndices.transformIndex);
    ConstantBuffer<MaterialConstants> material = GetConstantBuffer<MaterialConstants>(renderIndices.materialIndex);

    PS_INPUT output = (PS_INPUT)0;
    float4 pos = float4(posBuffer, 1.0f);
    pos = mul (mul ( mul( pos, wvpData.World ), wvpData.View), wvpData.Projection);
    output.Pos = pos;
    output.Pos.w = 1.0f;


    output.Color = float4(1,1,1,1);
    return output;
}
