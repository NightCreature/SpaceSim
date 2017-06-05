#ifndef PARTICLE_H
#define PARTICLE_H

#include "Math/vector3.h"
#include "Graphics/color.h"
#include "Graphics/interpolatecurveRGBA.h"


namespace ParticleSystem
{

struct ParticleData
{
    void createParticles(size_t numberOfParticles);
    void destroyParticles();
    void wakeParticle(size_t id);
    void killParticle(size_t id);
    void swapParticleData(size_t first, size_t second);

    Vector4* m_positionData;
    Vector4* m_velocityData;
    Vector4* m_startColor;
    Vector4* m_endColor;
    float* m_lifeTimes; //if these are zero or below particle is dead
    float* m_size;

    size_t m_aliveParticles;
    size_t m_maxParticles;
};

}
#endif