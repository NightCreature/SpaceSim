#pragma once

#include "Math/matrix44.h"
#include "Memory/AlignMacros.h"
#include "Core/Types/Types.h"

struct WVPBufferContent
{
    Matrix44 m_world;
    Matrix44 m_view;
    Matrix44 m_projection;
};

struct LightConstants
{
    ALIGN16 float m_position[4]; //Element 4 holds inner cone value
    ALIGN16 float m_direction[4]; //Element 4 holds outer cone value
    ALIGN16 float m_diffuse[4];
    ALIGN16 float m_specular[4];
    ALIGN16 float m_attenuationFallOff[4];
    ALIGN16 float m_range[4];//last three elements are empty
    ALIGN16 uint32 m_type[4];//Only the 1st value is used really
};

struct PerFrameConstants
{
    ALIGN16 LightConstants m_lightConstants[8];
    ALIGN16 float m_cameraPosition[4];
    ALIGN16 WVPBufferContent m_shadowMVP;
};

struct MaterialContent
{
    MaterialContent()
    {
        m_ambient[0] = 0.2f;
        m_ambient[1] = 0.2f;
        m_ambient[2] = 0.2f;
        m_ambient[3] = 1.0f;

        m_diffuse[0] = 0.8f;
        m_diffuse[1] = 0.8f;
        m_diffuse[2] = 0.8f;
        m_diffuse[3] = 1.0f;

        m_specular[0] = 0.0f;
        m_specular[1] = 0.0f;
        m_specular[2] = 0.0f;
        m_specular[3] = 1.0f;

        m_emissive[0] = 0.0f;
        m_emissive[1] = 0.0f;
        m_emissive[2] = 0.0f;
        m_emissive[3] = 1.0f;
        m_shininess = 1000.0f;
    }

    float m_ambient[4];
    float m_diffuse[4];
    float m_specular[4];
    float m_emissive[4];
    ALIGN16 float m_shininess;
};

struct CameraConstants
{
    Matrix44 m_matrix;
    Matrix44 m_inverseMatrix;
    Vector3 m_position;
};