#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include "vector3.h"
#include "Model.h"
#include "color.h"
#include <deque>

class Particle;
class ParticleSystemManager;

class ParticleEmitter
{
public:
	//enum EmitterType
	//{
	//	maincore,
	//	normal,
	//	system,
	//};

	ParticleEmitter(ParticleSystemManager* pfxManager);
	ParticleEmitter(ParticleSystemManager* pfxManager, const Vector3& position, const Vector3& normal);
	ParticleEmitter(ParticleSystemManager* pfxManager, const Vector3& position, const Model* emitter);
	~ParticleEmitter();

	void cleanup() {}

	void proceed(float elapsedtime);

	bool isAlive() const {return m_alive; }
	const Vector3& getPosition() const {return m_position; }
	float getStartVelocity() const {return m_startvelocity; }
	void setAlive(bool alive);
	void setPosition(const Vector3& position);
	void setStartVelocity(float startvelocity);
	void setParticleLifetime(float lifetime) {m_lifetime = lifetime;}
	void setParticleStartColor(const Color& initialcolor) {m_initialcolor = initialcolor;}
	void setParticleKey1Color(const Color& key1) {m_key1 = key1;}
	void setParticleKey2Color(const Color& key2) {m_key2 = key2;}
	void setParticelEndColor(const Color& endcolor) {m_endcolor = endcolor;}
	void setEmissionRate(float emissionrate) {m_emissionrate = emissionrate;}
	void setMaxParticles(float maxparticles) {m_maxparticles = maxparticles;}
	void setLifeTime(float lifetime) {m_emitterlifetime = lifetime;}
	void setSpreadAngle(float angle) {m_spreadangle = angle;}
	void setParticleSize(float size) {m_particlesize = size;}
	void setParticleKey1Pos(float pos) {m_key1pos = pos;}
	void setParticleKey2Pos(float pos) {m_key2pos = pos;}
	//void setType(EmitterType type) {m_type = type;}
	//EmitterType getType() const {return m_type;}

	void draw();
protected:
private:
	typedef std::deque<Particle* > ParticleBuffer;
	typedef std::deque<Particle* >::iterator ParticleBufferIT;

	ParticleBuffer m_particlebuffer;
	Vector3 m_position;
	Vector3 m_normal;
	Color m_initialcolor;
	Color m_key1;
	Color m_key2;
	Color m_endcolor;
    ParticleSystemManager* m_pfxManager;
	float m_startvelocity;
	float m_maxparticles;
	float m_emissionrate;
	float m_lifetime;
	float m_spreadangle;
	float m_emitterlifetime;
	float m_particlesize;
	float m_key1pos;
	float m_key2pos;
	int m_numparticles;
	int m_numemit;
    bool m_alive;
};
#endif