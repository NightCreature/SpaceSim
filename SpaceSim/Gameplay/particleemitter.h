#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include "Math/vector3.h"
#include "Gameplay/particle.h"
#include "Graphics/Model.h"
#include "Graphics/color.h"
#include <array>
#include <deque>

class Resource;

namespace ParticleSystem
{

class IGenerator
{
public:
    virtual ~IGenerator() = default;
    virtual void generate(size_t start, size_t end, const ParticleData& particleData) = 0;
};

class PointGenerator : public IGenerator
{
public:
    virtual void generate(size_t start, size_t end, const ParticleData& particleData);

private:
    Vector4 m_spawnPosition;
};

class IUpdater
{
public:
    virtual ~IUpdater() = default;
    virtual void update(double elapsedTime, const ParticleData& particleData) = 0;
};

class EulerUpdater : public IUpdater
{
public:
    EulerUpdater() { m_acceleration = Vector4(0.1f, 0.1f, 0.0f, 0.0f); }
    virtual void update(double elapsedTime, const ParticleData& particleData) override;
private:
    Vector4 m_acceleration;
};

struct ParticleEmitterData
{
    size_t m_maxNumberOfParticles = 25;
    size_t m_emissionRate;
    //Should really fix this to be something like generator and updater data instead of newing them here
    static constexpr size_t maxNumberOf = 32;
    std::array<IGenerator*, maxNumberOf> m_generators; //only allow 32 for now
    std::array<IUpdater*, maxNumberOf> m_updaters; //only allow 32 for now
};

class ParticleEmitterComponentBased : public RenderInterface
{
public:
    ~ParticleEmitterComponentBased();

    void initialise(Resource* resource);

    void update(double elapsedTime, const Matrix44& view, const Matrix44& projection, const Matrix44& inverseView);

    void addGenerator(IGenerator* generator) { m_generators.push_back(generator); }
    void addUpdater(IUpdater* updater) { m_updaters.push_back(updater); }

    void UpdateCbs() override;
    void PopulateCommandlist(Resource* resource, CommandList& commandList) override;

private:
    ParticleData m_particleData;
    std::vector<IGenerator*> m_generators;
    std::vector<IUpdater*> m_updaters;

    struct PartticleChacheData
    {
        float x, y, z, w;
        float r, g, b, a;
        float size;
    };
    std::vector<PartticleChacheData> m_cache; //to avoid reallocating this each update

    Resource* m_resource;

    float m_emmisionRate;

    ID3D11Buffer* m_particleDataBuffer;
    ID3D11Buffer* m_indexBuffer;
    ID3D11ShaderResourceView* m_srv;
    //Test code
    ID3D11Buffer* m_constantBuffers[2];
};

}
#endif