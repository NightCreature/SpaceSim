#include "CommonConstantBuffers.ivs"
#include "rootsignatures.ifx"


#include "BindlessBuffers.ifx"
#include "Rootsignatures.ifx"

ConstantBuffer<MeshResourceIndices> resourceIndices : register(b0);

//--------------------------------------------------------------------------------------
struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    //float4 Color : COLOR;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
[RootSignature(bindlessRS)]
PS_INPUT vs_main( uint id:SV_VERTEXID )
{
    float3 posBuffer = GetInstanceFromBufferT<float3>(resourceIndices.posBufferIndex, id);
    float4 colorBuffer = GetInstanceFromBufferT<float4>(resourceIndices.vertexColorBufferIndex, id);
    ConstantBuffer<WVPData> wvpData = GetConstantBuffer<WVPData>(resourceIndices.transformIndex);
    ConstantBuffer<WVPData> perScene = GetConstantBuffer<WVPData>(resourceIndices.sceneTransformIndex);

    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul( float4(posBuffer, 1), wvpData.World );
    output.Pos = mul( output.Pos, perScene.View );
    output.Pos = mul( output.Pos, perScene.Projection );

    //output.Color = colorBuffer;

    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 ps_main( PS_INPUT input) : SV_Target
{
    //float4 color = input.Color;
    float4 color = float4(1,0,0,1);
    return color;
}
