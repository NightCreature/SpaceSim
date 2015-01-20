#ifndef PARTICLE_SYSTEM_FX
#define PARTICLE_SYSTEM_FX

//float3 g_translation;//due to having screen aligned quads we dont need a world matrix
texture  g_particleTexture;
texture  g_particlePositionTexture;
texture  g_particleInitialVelocityTexture;
float4x4 g_projection;
float4x4 g_view;
float4x4 g_world;
float4   g_position;//Start position for unborn particles
float4   g_currElapsedTime;//x = current time in milliseconds, y = elapsedtime since last frame, z = size modifier ,w no value yet
bool     g_initialize;

sampler particleColorSampler = 
sampler_state
{
    Texture = <g_particleTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

sampler particlePositionSampler = 
sampler_state
{
    Texture = <g_particlePositionTexture>;
    MipFilter = NONE;
    MinFilter = POINT;
    MagFilter = POINT;
};

sampler particleInitialVelocitySampler = 
sampler_state
{
    Texture = <g_particleInitialVelocityTexture>;
    MipFilter = NONE;
    MinFilter = POINT;
    MagFilter = POINT;
};

struct VS_INPUT
{
    float3 position : POSITION0;
};

struct VS_OUPUT
{
    float4 Position : POSITION;
    float  particleSize : PSIZE;
    float2 texCoord : TEXCOORD0;
    float4 worldPos : TEXCOORD1;
};

struct VS_OUPUT_UPDATING 
{
    float4 pos : POSITION;
    float2 texCoord : TEXCOORD0;
};

struct PS_INPUT
{
    float4 Position : POSITION;
    float  particleSize : PSIZE;
    float2 texCoords : TEXCOORD0;
    float4 worldPos : TEXCOORD1;
};

struct PS_OUTPUT_UPDATING
{
    float4 Position : COLOR0;
};

struct PS_OUTPUT_RENDERING 
{
    float4 color : COLOR0;
};

VS_OUPUT_UPDATING vsUpdatePass(VS_INPUT In)
{
    VS_OUPUT_UPDATING output = (VS_OUPUT_UPDATING)0;
    output.pos = float4(In.position.xy, 0.0, 1.0);
    output.texCoord = In.position.xy;
    return output;
}

PS_OUTPUT_UPDATING updateVelPos(VS_OUPUT_UPDATING In) //actually returns float values
{
	PS_OUTPUT_UPDATING output = (PS_OUTPUT_UPDATING)0;
	
	
    float4 position = tex2Dlod(particlePositionSampler, float4(In.texCoord, 0, 0)); //consists of currentlife time, position data
	float4 initialData = tex2Dlod(particleInitialVelocitySampler, float4(In.texCoord, 0, 0)); //initial data consists of a birth modifier and initial velocities
	
	//if (g_initialize)
	//{
	//    output.Position.a = initialData.a;
	//    output.Position.rgb = g_position.xyz;
	//}
	//else
	if (position.a <= 0.0)
	{
		////Particle is dead and needs to respawn
		output.Position.a = initialData.a;
		output.Position.rgb = g_position.xyz;
	}
	else
	{
		//particle is still alice and needs updating
		float3 newVelocity = initialData.rgb * g_currElapsedTime.y;
		output.Position.rgb = position.rgb + newVelocity;
		output.Position.a -= g_currElapsedTime.y;
	}
	
    return output;
}


VS_OUPUT vsMain(VS_INPUT In) //Only used on pass two
{
    VS_OUPUT output = (VS_OUPUT)0;

    //Fetch the position from the texture
	float4x4 worldviewprojection = mul( g_world, mul( g_view, g_projection ) );
	
	float4 pos = float4( tex2Dlod(particlePositionSampler, float4(In.position.xy, 0.0, 0)).rgb, 1.0);
	output.Position = mul(pos, worldviewprojection);
	output.texCoord = float2(0.0, 0.0);
	output.particleSize = 3.0 * 10;
    output.worldPos = output.Position;
    
    return output;
}

float4 psMain(PS_INPUT In) : COLOR0
{
    float4 color = float4(In.texCoords, 0.0, 0.0);//complete black and no alpha particle for output
    
    color = tex2D(particleColorSampler, In.texCoords) * float4(1.0, 1.0, 0.0, 1.0);
    
    return color;
}

//Define the technique
technique particleSystem
{
	pass pass0
	{
		VertexShader = compile vs_3_0 vsUpdatePass();
		PixelShader  = compile ps_3_0 updateVelPos();
	}
    pass pass1
    {
        PointSpriteEnable = true;
        ZWriteEnable = false;
        AlphaBlendEnable = true;
        SrcBlend = SrcAlpha;
        DestBlend = One;
        VertexShader = compile vs_3_0 vsMain();
        PixelShader  = compile ps_3_0 psMain();
        ZWriteEnable = true;
        AlphaBlendEnable = false;
    }
}

#endif