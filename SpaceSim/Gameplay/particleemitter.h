#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include "Math/vector3.h"
#include "Gameplay/particle.h"
#include "Graphics/Model.h"
#include "Graphics/color.h"
#include <deque>

namespace ParticleSystem
{

class IGenerator
{
public:
    virtual void generate(size_t start, size_t end, const ParticleData& particleData) = 0;
};

class IUpdater
{
public:
    virtual void update(double elapsedTime, const ParticleData& particleData) = 0;
};

struct ParticleEmitterData
{
    Vector3 m_emitterDirection;
    Vector3 m_emitterPosition;
    size_t m_maxNumberOfParticles = 25;
    size_t m_emissionRate;
    float m_particleLifetime;
    float m_particleSize;
    float m_startVelocity;
    float m_emitConeAngle;
};

class ParticleEmitterComponentBased
{
public:

    void update(double elapsedTime);
private:
    ParticleData m_particleData;
    std::vector<IGenerator*> m_generators;
    std::vector<IUpdater*> m_updaters;

    float m_emmisionRate;
};

}
#endif