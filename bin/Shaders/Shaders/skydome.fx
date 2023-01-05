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
    float3 Nor : NORMAL0;
    float2 Tex : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
[RootSignature(bindlessRS)]
PS_INPUT vs_main( uint id:SV_VERTEXID )
{
    float4 pos = float4(GetInstanceFromBufferT<float3>(resourceIndices.posBufferIndex, id),0);
    WVPData wvpData = GetInstanceFromBuffer<WVPData>(resourceIndices.transformIndex);

    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul( pos, wvpData.World );
    output.Pos = mul( output.Pos, wvpData.View );
    output.Pos = mul( output.Pos, wvpData.Projection );

    output.Nor = GetInstanceFromBufferT<float3>(resourceIndices.normalBufferIndex, id);
    output.Tex = GetInstanceFromBufferT<float2>(resourceIndices.textureBufferIndex, id);
    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 ps_main(PS_INPUT input) : SV_Target
{
    Texture2D<float4> shaderTextures = GetColorTexture(resourceIndices.albedoMapIndex);
    return saturate( shaderTextures.Sample(linearWrapSampler, input.Tex) );
}