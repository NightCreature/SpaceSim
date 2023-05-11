#ifdef __cplusplus
#pragma once
#include "Core/Types/Types.h"
#include <limits>
#endif

struct MeshResourceIndices
{
#ifdef __cplusplus
	MeshResourceIndices()
	{
		memset(this, std::numeric_limits<uint>::max(), sizeof(MeshResourceIndices));
	}
#endif
	//mesh information
	uint posBufferIndex;
	uint textureBufferIndex;
	uint normalBufferIndex;
	uint tangentBufferIndex;
	uint vertexColorBufferIndex;

	uint numberOfTextureStreams;

	//transform information
	uint transformIndex;
	uint sceneTransformIndex; //This is view projection

	uint materialIndex;
	
	//textures information
	uint shadowMapIndex;
	uint shadowSamplerIndex;

	uint albedoMapIndex;
	uint albedoSamplerIndex;
	
	uint normalMapIndex;
	uint normalSamplerIndex;

	uint cubeReflectionIndex;

	uint ShadowProjection;

	uint emmisiveMap;

	//Light info
	uint LightBufferIndex;
	uint NumberOfLights; //This is now in the light data buffer

	//Camera info
	uint CameraBufferIndex;
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