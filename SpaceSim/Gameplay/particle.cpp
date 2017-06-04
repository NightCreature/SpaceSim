#include "Gameplay/particle.h"
#include "Math/vector4.h"
#include <iostream>
#include <algorithm>

namespace ParticleSystem
{
//
//Particle::Particle()
//{
//}
//
//Particle::~Particle()
//{
//}
//
//void Particle::initialize(const Vector3& position, const Vector3& velocity, const Color& startcolor, float lifetime)
//{
//    m_state = Unborn;
//    m_position = position;
//    m_velocity = velocity;
//    m_startcolor = startcolor;
//    m_intercolor = startcolor;
//    m_lifetime = lifetime;
//    m_living = 0.0f;
//}
//
//void Particle::initializeColorCurve(Color& color1, Color& color2, Color& color3, Color& color4, float keyframe1, float keyframe2)
//{
//    Vector4 v1(color1.rgba()[0], color1.rgba()[1], color1.rgba()[2], color1.rgba()[3]),
//        v2(color2.rgba()[0], color2.rgba()[1], color2.rgba()[2], color2.rgba()[3]),
//        v3(color3.rgba()[0], color3.rgba()[1], color3.rgba()[2], color3.rgba()[3]),
//        v4(color4.rgba()[0], color4.rgba()[1], color4.rgba()[2], color4.rgba()[3]);
//    m_icrgba = interpolateCurveRGBA(v1, v2, v3, v4, keyframe1, keyframe2);
//}
//
//void Particle::proceed(float elapsedtime)
//{
//    static Vector3 windVelocity;
//    static Vector3 finalVelocity;
//
//    switch (m_state)
//    {
//    case Unborn:
//        if (m_living <= m_lifetime)
//        {
//            m_state = Living;
//        }
//        break;
//
//    case Living:
//        if (m_living >= m_lifetime)
//        {
//            m_state = Dead;
//        }
//        else
//        {
//            m_living += elapsedtime;
//            float relage = getRelativeAge();
//            Vector4 interpolated = m_icrgba.GetValue(relage);
//            m_intercolor = Color(interpolated.x(), interpolated.y(), interpolated.z(), interpolated.w());
//
//            // update velocity
//            m_velocity += m_velocity * elapsedtime;
//            m_position = m_position + m_velocity * elapsedtime;
//        }
//        break;
//
//    default:
//        break;
//    }
//}
//
//float Particle::getRelativeAge() const
//{
//    return m_living / m_lifetime;
//}

//-----------------------------------------------------------------------------
//! @brief   
//! @remark
//-----------------------------------------------------------------------------
void ParticleData::createParticles(size_t numberOfParticles)
{
    m_maxParticles = numberOfParticles;
    m_aliveParticles = 0;

    m_positionData = new Vector4[m_maxParticles];
    m_velocityData = new Vector4[m_maxParticles];
    m_startColor = new Vector4[m_maxParticles];
    m_endColor = new Vector4[m_maxParticles];
    m_lifeTimes = new float[m_maxParticles];
    m_size = new float[m_maxParticles];
}

//-----------------------------------------------------------------------------
//! @brief   
//! @remark
//-----------------------------------------------------------------------------
void ParticleData::destroyParticles()
{
    m_maxParticles = 0;
    m_aliveParticles = 0;

    delete [] m_positionData;
    delete [] m_velocityData;
    delete [] m_startColor;
    delete [] m_endColor;
    delete [] m_lifeTimes;
    delete [] m_size;
}

//-----------------------------------------------------------------------------
//! @brief   
//! @remark
//-----------------------------------------------------------------------------
void ParticleData::wakeParticle(size_t id)
{
    if (0 < id && id < m_maxParticles && m_aliveParticles < m_maxParticles)
    {
        swapParticleData(id, m_aliveParticles++);
    }
}

//-----------------------------------------------------------------------------
//! @brief   
//! @remark
//-----------------------------------------------------------------------------
void ParticleData::killParticle(size_t id)
{
    if (0 < id && id < m_maxParticles)
    {
        swapParticleData(id, m_aliveParticles--);
    }
}

//-----------------------------------------------------------------------------
//! @brief   
//! @remark
//-----------------------------------------------------------------------------
void ParticleData::swapParticleData(size_t first, size_t second)
{
    std::swap(m_positionData[first], m_positionData[second]);
    std::swap(m_velocityData[first], m_velocityData[second]);
    std::swap(m_startColor[first], m_startColor[second]);
    std::swap(m_endColor[first], m_endColor[second]);
    std::swap(m_lifeTimes[first], m_lifeTimes[second]);
    std::swap(m_size[first], m_size[second]);
}

}