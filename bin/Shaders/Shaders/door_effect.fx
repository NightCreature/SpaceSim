#include "BindlessBuffers.ifx"
#include "CommonStructures.ifx"
#include "rootsignatures.ifx"
#include "Samplers.ifx"

ConstantBuffer<MeshResourceIndices> resourceIndices : register(b0);

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
};

// function for getting the checker pattern
float4 checker(float2 uv) 
{
    float checkSize = 4;
    float2 t = abs(  floor(uv) );
    int2 s = floor(t * 2);
    if (floor(s[0] + s[1]) % 2 == 0) 
    {
        return float4(0, 0, 0, 1);  
    } 
    else
    {
        return float4(1, 1, 1, 1);  
    }
}

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
[RootSignature(bindlessRS)]
PS_INPUT vs_main( uint vertexID : SV_VertexID )
{
    float3 pos = GetInstanceFromBufferT<float3>(resourceIndices.posBufferIndex, vertexID);
    ConstantBuffer<WVPData> wvpData = GetConstantBuffer<WVPData>(resourceIndices.transformIndex);
    ConstantBuffer<WVPData> perScene = GetConstantBuffer<WVPData>(resourceIndices.sceneTransformIndex);

    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul( float4(pos,1.0f), wvpData.World );
    output.Pos = mul( output.Pos, perScene.View );
    output.Pos = mul( output.Pos, perScene.Projection );

    output.Tex = GetInstanceFromBufferT<float2>(resourceIndices.textureBufferIndex, vertexID ); 
    return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 ps_main( PS_INPUT input) : SV_Target
{
    Texture2D<float4> shaderTextures = GetColorTexture(resourceIndices.albedoMapIndex);
    ConstantBuffer<MaterialConstants> material = GetConstantBuffer<MaterialConstants>(resourceIndices.materialIndex);
    float4 color = material.ambient + shaderTextures.Sample(linearWrapSampler, input.Tex) * material.diffuse + material.emissive;
    color.a = shaderTextures.Sample(linearWrapSampler, input.Tex).a;
    return saturate(color);
}
