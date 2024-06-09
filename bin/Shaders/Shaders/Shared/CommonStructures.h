#pragma once
#ifdef __cplusplus
#include "Math/VectorTypes.h"
#include "Math/matrix44.h"

#include "imgui.h"

#include <limits>
#include <string>

using float3 = VectorBySize<float, 3>;
using float4 = VectorBySize<float, 4>;
using uint4 = VectorBySize<uint32, 4>; //So we have a type that matches the uint4 type in HLSL
using float4x4 = Matrix44;
#endif
//This file is shared between shaders and c++

struct LightConstants
{
    float4 m_position;
    float4 m_direction;
    float4 m_diffuse;
    float4 m_specular;
    float4 m_attenuationFallOff;
    float4 m_range;
    float4 m_irradiance;
    uint4 m_type;
};

struct WVPData
{
	float4x4 World;
	float4x4 View;
	float4x4 Projection;
};

struct MaterialConstants
{
#ifdef __cplusplus
    MaterialConstants()
        : ambient({ 0.25f, 0.25f, 0.25f, 1.0f })
        , diffuse({ 1.0f, 1.0f, 1.0f, 1.0f })
        , specular({ 1.0f, 1.0f, 1.0f, 1.0f })
        , emissive({ 0.0f, 0.0f, 0.0f, 1.0f })
        , shininess(100.0f)
    {
    }

    void OnDebugImgui(const std::string_view& name)
    {
        ImGui::CollapsingHeader(name.data());
        ImGui::InputFloat4("Ambient", ambient.GetDataPtr());
        ImGui::InputFloat4("Diffuse", diffuse.GetDataPtr());
        ImGui::InputFloat4("Specular", specular.GetDataPtr());
        ImGui::InputFloat4("Emissive", emissive.GetDataPtr());
        ImGui::InputFloat("Shininess", &shininess);
    }
#endif

    float4 ambient;
    float4 diffuse;
    float4 specular;
    float4 emissive;
    float shininess;
};

struct LightParameters
{
    LightConstants m_lights[32];
    uint NumberOfLights;
};

struct CameraConstants
{
    float4x4 m_matrix;
    float4x4 m_inverseMatrix;
    float3 m_position;
};

struct CameraConstantArray
{
    CameraConstants m_constants[32];
};
