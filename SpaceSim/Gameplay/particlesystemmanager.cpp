#include "particlesystemmanager.h"
#include "particleemitter.h"
#include "particle.h"
#include <cstdlib>
#include <ctime>

ParticleSystemManager::ParticleSystemManager()
{
	//for (int i = 0; i < MAX_PARTICLES; i++)
	//	m_allparticles.push_back(new Particle());
	//ParticlePoolIt ppit = m_allparticles.begin();
	//while (ppit != m_allparticles.end())
	//{
	//	Particle* p = *ppit;
	//	m_freeparticles.push_back(p);
	//	ppit++;
	//}
	//for (int i = 0; i < MAX_RANDOMfloat; i++)
	//{
	//	float randomfloat = (2.0f*((float)rand())/((float)RAND_MAX))-1.0f;
	//	m_randomfloats.push_back(randomfloat);
	//}
	//for (int i = 0; i < MAX_RANDOMint; i++)
	//	m_randomints.push_back(rand());

	m_intcounter = 0;
	m_floatcounter = 0;
}

ParticleSystemManager::~ParticleSystemManager()
{
	for(unsigned int index = 0; index < m_allemitters.size(); index++)
	{
		ParticleEmitter* pe = m_allemitters.at(index);
		delete pe;
	}
	if (!m_allparticles.empty())
	{
		for (ParticlePoolIt ppit = m_allparticles.begin(); ppit != m_allparticles.end(); ++ppit )
		{
			Particle* p = *ppit;
			delete p;
            p = nullptr;
		}
		m_allparticles.clear();
	}
}

void ParticleSystemManager::cleanup()
{
	for(unsigned int index = 0; index < m_aliveemitters.size(); index++)
	{
		ParticleEmitter* pe = m_aliveemitters.at(index);
		pe->setAlive(false);
		//delete pe;
	}
	m_aliveemitters.clear();
	for(unsigned int index = 0; index < m_allemitters.size(); index++)
	{
		ParticleEmitter* pe = m_allemitters.at(index);
		delete pe;
	}
	m_allemitters.clear();
}

ParticleEmitter* ParticleSystemManager::createEmitter(const Vector3& position, const Vector3& normal)
{
	m_allemitters.push_back(new ParticleEmitter(this, position, normal));
	EmitterPoolIt emit = m_allemitters.end();
	emit--;
	return *emit;
}

bool ParticleSystemManager::registerAliveEmitter(ParticleEmitter* emitter)
{
	if (MAX_EMITTERS > m_allemitters.size())
	{
		EmitterPoolIt it = m_aliveemitters.begin();
		while (it != m_aliveemitters.end())
		{
			if (*it == emitter)
				break;
			it++;
		}
		if (it != m_aliveemitters.end())
			return false;
		else
			m_aliveemitters.push_back(emitter);
		return true;
	}
	return false;
}

bool ParticleSystemManager::deregisterEmitter(ParticleEmitter* emitter)
{
	if (0 != m_allemitters.size())
	{
		EmitterPoolIt it = m_allemitters.begin();
		while (it != m_allemitters.end())
		{
			if (*it == emitter)
			{
				m_allemitters.erase(it);
				delete emitter;
				deregisterAliveEmitter(emitter);
				break;
			}
			it++;
		}
		return true;
	}
	return false;
}

bool ParticleSystemManager::deregisterAliveEmitter(ParticleEmitter* emitter)
{
	if (0 != m_allemitters.size())
	{
		for (unsigned int i = 0; i < m_aliveemitters.size(); i++)
		{
			if (m_aliveemitters[i] == emitter)
			{
				//ParticleEmitter* pe = m_allemitters.at(i);
				//delete pe;
				m_aliveemitters[i] = 0;
				break;
			}
		}
		return true;
	}
	return false;
}

Particle* ParticleSystemManager::giveParticle()
{
	if (0 != m_freeparticles.size())
	{
		Particle* p = m_freeparticles.back();
		m_freeparticles.pop_back();
		return p;
	}
	return 0;
}

void ParticleSystemManager::returnParticle(Particle *p)
{
	m_freeparticles.push_back(p);
}

void ParticleSystemManager::proceed(float elapsedtime)
{
	if (!m_aliveemitters.empty())
	for (unsigned int i = 0; i < m_aliveemitters.size(); i++)
	{
		if (0 != m_aliveemitters[i])
			m_aliveemitters[i]->proceed(elapsedtime);
	}
}

void ParticleSystemManager::drawSystems()
{
	if (!m_aliveemitters.empty())
	for (unsigned int i = 0; i < m_aliveemitters.size(); i++)
	{
		if (0 != m_aliveemitters[i])
			m_aliveemitters[i]->draw();
	}
}

int ParticleSystemManager::randomint()
{
	m_intcounter++;
	m_intcounter %= MAX_RANDOMint; 
	return m_randomints[m_intcounter];
}

float ParticleSystemManager::randomfloat()
{
	m_floatcounter++;
	m_floatcounter %= MAX_RANDOMfloat; 
	return m_randomfloats[m_floatcounter];
}

Vector3 ParticleSystemManager::randomVector3()
{
	return Vector3(randomfloat(), randomfloat(), randomfloat());
}

