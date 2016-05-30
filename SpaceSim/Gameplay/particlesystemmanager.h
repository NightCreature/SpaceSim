#ifndef PARTICLESYSTEMMANAGER_H
#define PARTICLESYSTEMMANAGER_H

#include <vector>
#include "Math/vector3.h"

class ParticleEmitter;
class Particle;

const int MAX_PARTICLES	  = 250000;
const int MAX_EMITTERS	  = 2048;
const int MAX_RANDOMint	  = 65536;
const int MAX_RANDOMfloat = 1048576;

class ParticleSystemManager 
{
public:
	ParticleSystemManager();
	~ParticleSystemManager();

	void cleanup();

	void proceed(float elapsedtime);
	void drawSystems();

	ParticleEmitter* createEmitter(const Vector3& position, const Vector3& normal);
	//ParticleEmitter* createEmitter(const Vector3& position, const Shape* emitter);
	bool registerAliveEmitter(ParticleEmitter* emitter);
	bool deregisterEmitter(ParticleEmitter* emitter);
	bool deregisterAliveEmitter(ParticleEmitter* emitter);
	void returnParticle(Particle* p);
	Particle* giveParticle();
	int 	randomint();
	float 	randomfloat();
	Vector3 randomVector3();
	int getNumberUsedParticles() const {return (int)(MAX_PARTICLES - m_freeparticles.size());}
protected:
private:
	//Vector typedefs
	typedef std::vector<ParticleEmitter*> EmitterPool;
	typedef std::vector<Particle*> ParticlePool;
	typedef std::vector<float> Randomfloats;
	typedef std::vector<int> Randomints;

	//Vector iterator typedef
	typedef std::vector<ParticleEmitter*>::iterator EmitterPoolIt;
	typedef std::vector<Particle*>::iterator ParticlePoolIt;
	typedef std::vector<float>::iterator RandomfloatsIt;
	typedef std::vector<int>::iterator RandomintsIt;

	EmitterPool  m_allemitters;
	EmitterPool  m_aliveemitters;
	ParticlePool m_allparticles;
	ParticlePool m_freeparticles;
	Randomfloats m_randomfloats;
	Randomints	 m_randomints;
	int			 m_intcounter;
	int			 m_floatcounter;
};
#endif
