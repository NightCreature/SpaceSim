//--------------------------------------------------------------------------------------
// File: 3DVirtualPaintBrush.fx
//
// The effect file for a virtual paint brush application  
// 
// Copyright (c) Paul "NightCreature" Kruitz. All rights reserved.
//--------------------------------------------------------------------------------------
#include "phong_lighting_shader.ifx"
#include "cube_mapping.ifx"
#include "normal_mapping.ifx"
//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
float4 g_ambient;      // Material's ambient color
float4 g_diffuse;      // Material's diffuse color
float4 g_specular;
float4 g_emissive;
float g_specpower;
float3 g_eyeposition;
float3 g_lightposition;
float4x4 g_world;                 // World matrix for object
float4x4 g_view;
float4x4 g_worldviewprojection;   // World * View * Projection matrix
texture g_texture;                // Color texture for mesh
texture g_normalTexture;
texture g_heightTexture;
float g_lightStrength = 15.0;
float g_fallOffFactor = 1.0;
float g_innerConeAngle = 2.5 * (3.141592 / 180);  ; //Needs to be radian
float g_outerConeAngle = 35.0 * (3.141592 / 180); //Needs to be radian
float3 g_lightLookAt = float3(0.0, 0.0, 0.0);
float4 g_lightColor;

//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
sampler MeshTextureSampler = 
sampler_state
{
    Texture = <g_texture>;
    MipFilter = LINEAR;
    MinFilter = ANISOTROPIC;
    MagFilter = LINEAR;
};

sampler normalSampler = 
sampler_state
{
    Texture = <g_normalTexture>;
    MipFilter = LINEAR;
    MinFilter = ANISOTROPIC;
    MagFilter = LINEAR;
};

sampler heightSampler = 
sampler_state
{
    Texture = <g_heightTexture>;
    MipFilter = LINEAR;
    MinFilter = ANISOTROPIC;
    MagFilter = LINEAR;
};

samplerCUBE CubeSampler = 
sampler_state
{
    Texture = <g_texture>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
};

//--------------------------------------------------------------------------------------
// Vertex shader input structure
//--------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 position : POSITION;
	float3 normal   : NORMAL;
	float3 tangent  : TANGENT;
	float2 texcoord : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex shader output structure
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position       : POSITION;   // vertex position 
	float3 Normal	      : NORMAL;
    float2 TextureUV  	  : TEXCOORD0;  // vertex texture coords 
	float3 ViewDirection  : TEXCOORD1; //Direction from vertex to eye
	float3 LightDirection : TEXCOORD2; //Direction from vertex to ligth position
	float3 WorldPosition  : TEXCOORD3;
	float3 Tangent		  : TEXCOORD4;
};

struct VS_INPUT_PN
{
    float4 position : POSITION;
    float3 normal   : NORMAL;
};

struct VS_OUTPUTCM
{
    float4 Position      : POSITION;
    float3 Normal        : NORMAL;
    float3 ViewDirection : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Pixel shader output structure
//--------------------------------------------------------------------------------------
struct PS_OUTPUT
{
    float4 color : COLOR0;  // Pixel color    
};

MaterialData createMaterialData() 
{
    MaterialData md;
    md.g_ambient = g_ambient;
    md.g_diffuse = g_diffuse;
    md.g_specular = g_specular;
    md.g_emissive = g_emissive;
    md.g_specpower = g_specpower * 25.0;
    return md;
}

//--------------------------------------------------------------------------------------
// This shader computes standard transform and lighting
//--------------------------------------------------------------------------------------
VS_OUTPUT RenderSceneVS( VS_INPUT In)
{
    VS_OUTPUT Output;   
    Output.Position = mul(In.position, g_worldviewprojection);    
	Output.Normal = In.normal;//mul(In.normal, g_world);
    Output.TextureUV = In.texcoord;
	float4 vertex = mul(In.position, transpose(g_world));
	Output.ViewDirection =  g_eyeposition - vertex.xyz;
	Output.LightDirection = g_lightposition - vertex.xyz;
	Output.WorldPosition = vertex.xyz;
	Output.Tangent = In.tangent;
    return Output;    
}

//--------------------------------------------------------------------------------------
// This shader outputs the pixel's color by modulating the texture's
//       color with diffuse material color
//--------------------------------------------------------------------------------------
PS_OUTPUT RenderScenePS( VS_OUTPUT In) 
{ 
    PS_OUTPUT Output = (PS_OUTPUT)0;
	
    float attenuation = g_lightStrength / abs(sqrt(dot(g_lightposition - In.WorldPosition, g_lightposition - In.WorldPosition)));
    float2 textureCoords = In.TextureUV;
	//textureCoords = getParallaxTexCoords(heightSampler, In.TextureUV, In.ViewDirection, 0.08, 0.04, 2);
    float4 texColor = tex2D(MeshTextureSampler, textureCoords) * float4(1.0, 0.5, 0.0, 1.0);
    //texColor = float4(1.0, 1.0, 1.0, 1.0);
    saturate(texColor);

    float3 lightDirection = In.WorldPosition.xyz - g_lightposition;
	//Create the inverse tangent matrix
    
    float3 normal = normalize( -getNormalNormalMapping( normalSampler, textureCoords));
    normal = normalize(In.Normal);
    getWorldNormal(In.Normal, In.Tangent, g_world, normal);

    Output.color = PhongShadingSpotLight(normal, In.LightDirection, In.ViewDirection, g_lightLookAt - g_lightposition , g_innerConeAngle, g_outerConeAngle, g_fallOffFactor, attenuation, texColor, createMaterialData(), g_lightColor);
    Output.color += PhongShadingPointLight(normal, In.LightDirection, In.ViewDirection, texColor, attenuation, createMaterialData(), float4(1.0, 1.0, 1.0, 1.0));
	//Output.color = PhongShadingDirectionalLight( normal, In.LightDirection, In.ViewDirection, texColor, createMaterialData(), g_lightColor);
    if (Output.color.a == 0)
        Output.color = float4(0.0, 0.0, 0.0, 0.0);
    Output.color.a = texColor.a;
	saturate(Output.color);//Have all values between 0..1
    return Output;
}

//--------------------------------------------------------------------------------------
//Vertex shader for clock cube mapping
//--------------------------------------------------------------------------------------
VS_OUTPUTCM CubeMapSceneVS(VS_INPUT_PN Input)
{
    VS_OUTPUTCM Output = (VS_OUTPUTCM)0;
    Output.Position = mul(Input.position, g_worldviewprojection);
    Output.Normal  = mul(Input.normal, g_world);
    float4 vertex = mul(Input.position, g_world);
    Output.ViewDirection = vertex.xyz - g_eyeposition;
    return Output;
}

PS_OUTPUT CubeMapScenePS(VS_OUTPUTCM Input)
{
    PS_OUTPUT Output;
    Output.color = reflectionCubeMapping(Input.Normal, Input.ViewDirection, CubeSampler);
    saturate(Output.color);

    return Output;
}

//--------------------------------------------------------------------------------------
// Renders scene to render target
//--------------------------------------------------------------------------------------
technique RenderSceneWithTexture
{
    pass P0
    {        
        AlphaBlendEnable = true;
        SrcBlend = SrcAlpha;
        DestBlend = One; 
        ZEnable = false;
        VertexShader = compile vs_3_0 RenderSceneVS();
        PixelShader  = compile ps_3_0 RenderScenePS(); 
    }
}

technique reflectionMapping
{
    pass pass0
    {
        VertexShader = compile vs_3_0 CubeMapSceneVS();
        PixelShader  = compile ps_3_0 CubeMapScenePS();
    }
}

//technique simpleNormalRendering
//{
//    pass pass0
//    {
//        VertexShader = compile vs_3_0 SimpleNormalVS();
//        PixelShader  = compile ps_3_0 SimpleNormalPS();
//    }
//}
