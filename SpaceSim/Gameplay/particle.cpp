#include "Gameplay/particle.h"
#include "Math/vector4.h"
#include <iostream>
#include <algorithm>

namespace ParticleSystem
{
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