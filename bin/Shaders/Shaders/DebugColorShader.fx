#include "CommonConstantBuffers.ivs"
#include "rootsignatures.ifx"


#include "BindlessBuffers.ifx"
#include "Rootsignatures.ifx"

ConstantBuffer<MeshResourceIndices> renderIndices : register(b0);

//--------------------------------------------------------------------------------------
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
[RootSignature(bindlessRS)]
PS_INPUT vs_main( uint id:SV_VERTEXID )
{
    float3 posBuffer = GetInstanceFromBufferT<float3>(renderIndices.posBufferIndex, id);
    ConstantBuffer<WVPData> wvpData = GetConstantBuffer<WVPData>(renderIndices.transformIndex);

    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul( posBuffer, wvpData.World );
    output.Pos = mul( output.Pos, wvpData.View );
    output.Pos = mul( output.Pos, wvpData.Projection );

    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 ps_main( PS_INPUT input) : SV_Target
{
    return float4(1,0,0,1);
}
