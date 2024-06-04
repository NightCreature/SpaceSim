#pragma once
#ifdef __cplusplus
#include "Core/Types/Types.h"
#include <limits>
#endif
//This file is shared between shaders and c++
struct MeshResourceIndices
{
#ifdef __cplusplus
	MeshResourceIndices()
	{
		memset(this, std::numeric_limits<uint>::max(), sizeof(MeshResourceIndices));
	}
#endif
	//mesh information
	/*0*/uint posBufferIndex;
	/*1*/uint textureBufferIndex;
	/*2*/uint normalBufferIndex;
	/*3*/uint tangentBufferIndex;
	/*4*/uint vertexColorBufferIndex;

	/*5*/uint numberOfTextureStreams;

	//transform information
	/*6*/uint transformIndex;
	/*7*/uint sceneTransformIndex; //This is view projection

	/*8*/uint materialIndex;
	
	//textures information
	/*9*/uint shadowMapIndex;
	/*10*/uint shadowSamplerIndex;

	/*11*/uint albedoMapIndex;
	/*12*/uint albedoSamplerIndex;
	
	/*13*/uint normalMapIndex;
	/*14*/uint normalSamplerIndex;

	/*15*/uint cubeReflectionIndex;

	/*16*/uint ShadowProjection;

	/*17*/uint emmisiveMap;

	//Light info
	/*18*/uint LightBufferIndex;
	/*19*/uint NumberOfLights; //This is now in the light data buffer

	//Camera info
	/*20*/uint CameraBufferIndex;

	/*21*/uint roughnessMapIndex;
	/*22*/uint metalnessMapIndex;
	/*23*/uint specularMapIndex;
	/*24*/uint heightMapIndex;
	/*25*/uint opacityMapIndex;
	/*26*/uint ambientOcclusionMapIndex;
	/*27*/uint refractionMapIndex;
	/*28*/uint cubeImageBasedLightningMapIndex;
};

struct ParticleData
{
#ifdef __cplusplus
	ParticleData()
    {
        memset(this, std::numeric_limits<uint>::max(), sizeof(ParticleData));
    }
#endif
	uint positionIndex;
	uint colorIndex;
	uint sizeIndex;
	uint transformIndex;

	uint textureIndex;
};