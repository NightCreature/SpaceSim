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
    float4 pos = float4(GetInstanceFromBufferT<float3>(renderIndices.posBufferIndex, id),0);
    WVPData wvpData = GetInstanceFromBuffer<WVPData>(renderIndices.transformIndex);

    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul( pos, wvpData.World );
    output.Pos = mul( output.Pos, wvpData.View );
    output.Pos = mul( output.Pos, wvpData.Projection );

    output.Color = GetInstanceFromBuffer<MaterialConstants>(renderIndices.materialIndex).diffuse;
    return output;
}
