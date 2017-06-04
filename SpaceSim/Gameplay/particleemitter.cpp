#include "Gameplay/particleemitter.h"
#include "Gameplay/particlesystemmanager.h"
#include "Gameplay/particle.h"
#include "Gameplay/square.h"
#include "Math/matrix33.h"
#include "Graphics/texturemanager.h"
#include <iostream>

namespace ParticleSystem
{

//-----------------------------------------------------------------------------
//! @brief   
//! @remark
//-----------------------------------------------------------------------------
void ParticleEmitterComponentBased::update(double elapsedTime)
{
    size_t numberOfParticlesToGenerate = static_cast<size_t>(elapsedTime * m_emmisionRate);
    size_t startingParticleIndex = m_particleData.m_aliveParticles;
    size_t lastParticleIndex = std::min(startingParticleIndex + numberOfParticlesToGenerate, m_particleData.m_maxParticles - 1);

    for (auto&& generator : m_generators)
    {
        generator->generate(startingParticleIndex, lastParticleIndex, m_particleData);
    }

    for (size_t counter = startingParticleIndex; counter < lastParticleIndex; ++counter)
    {
        m_particleData.wakeParticle(counter);
    }

    for (auto&& updater : m_updaters)
    {
        updater->update(elapsedTime, m_particleData);
    }

    //Run a last updater that is fixed and checks if particles are alive still
    for (size_t counter = 0; counter < m_particleData.m_aliveParticles; ++counter)
    {
        if (m_particleData.m_lifeTimes[counter] < 0.0f)
        {
            m_particleData.killParticle(counter);
        }
    }
}

}