#include "particleemitter.h"
#include "particlesystemmanager.h"
#include "particle.h"
#include "square.h"
#include "matrix33.h"
#include "texturemanager.h"
#include <iostream>

ParticleEmitter::ParticleEmitter(ParticleSystemManager* pfxManager) :
m_pfxManager(pfxManager)
{
	m_numparticles = 0;
	//m_emittershape = NULL;
	m_normal = Vector3::zero();
}

ParticleEmitter::ParticleEmitter(ParticleSystemManager* pfxManager, const Vector3 &position, const Model *emitter) :
m_pfxManager(pfxManager)
{
    m_position = position;
    m_normal = Vector3::xAxis();
    m_numparticles = 0;
    (void*)emitter;
}

ParticleEmitter::ParticleEmitter(ParticleSystemManager* pfxManager, const Vector3 &position, const Vector3 &normal) :
m_pfxManager(pfxManager)
{
	m_position = position;
	m_normal = normal;
	m_numparticles = 0;
}

ParticleEmitter::~ParticleEmitter()
{
    for (ParticleBufferIT it = m_particlebuffer.begin(); it != m_particlebuffer.end(); ++it)
	{
		Particle* p = *it;
		m_pfxManager->returnParticle(p);
	}

    ParticleBuffer emptyBuffer;
    std::swap(m_particlebuffer, emptyBuffer);
}

void ParticleEmitter::proceed(float elapsedtime)
{
	Particle* p;
	if (m_alive)
	{
		//Remove dead particles
		ParticleBufferIT pbit = m_particlebuffer.begin();
		while (pbit != m_particlebuffer.end())
		{
			p = *pbit;
			if (p->getState() != Particle::Dead)
				break;
			pbit++;
			m_pfxManager->returnParticle(p);
			m_particlebuffer.pop_front();
			m_numparticles--;
		}
		//Update remaining if any particles
		if (!m_particlebuffer.empty())
		{
			pbit = m_particlebuffer.begin();
			while (pbit != m_particlebuffer.end())
			{
				p = *pbit;
				p->proceed(elapsedtime);
				pbit++;
			}
		}
		if (m_emitterlifetime > 0)
		{
			//decrement lifetime of emitter when gone delete it
			m_emitterlifetime -= elapsedtime;
			if (m_maxparticles > m_numparticles)
			{
				//emit particles
				for (int i = 0; i < m_emissionrate * elapsedtime; i++)
				{
					p = m_pfxManager->giveParticle();
					m_particlebuffer.push_back(p);
					Vector3 ortho1, ortho2;
					ortho1 = m_normal.findOrthogonal();
					ortho1.normalize();
					ortho2 = m_normal.cross(ortho1);   // cross product
					ortho2.normalize();
					float angle1 = m_pfxManager->randomfloat();
					float angle2 = m_pfxManager->randomfloat();
					//Calculate spread cone
					Matrix33 rotatemat;
					rotate(rotatemat, ortho1, m_spreadangle * angle1);
					Vector3 dir = m_normal * rotatemat;
					rotate(rotatemat, ortho2, m_spreadangle * angle2);
					dir = dir * rotatemat;
					dir.normalize();
					p->initialize(m_position, dir * m_startvelocity, m_initialcolor, m_lifetime);
					p->initializeColorCurve(m_initialcolor, m_key1, m_key2, m_endcolor, 0.33f, 0.66f);
					p->setSize(m_particlesize);
					m_numparticles++;
				}
			}
		}
		else
		{
			if (m_particlebuffer.empty())
			{
				setAlive(false);
			}
		}
	}
}

void ParticleEmitter::draw()
{
	ParticleBufferIT pbit = m_particlebuffer.begin();
	while (pbit != m_particlebuffer.end())
	{
		//Particle* p = *pbit;
		////draw particle
		//Vector3 pos = p->getPosition();
		//float size = p->getSize();
		//glDepthMask(false);
		////Set textures
		//TextureManager& tm = TextureManager::getInstance();
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glEnable(GL_TEXTURE_2D);
		//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		//glTexEnvf( GL_POint_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE );
		//tm.activateTexture(SpaceSim::getInstance().getstd::stringSetting("particle"));
		//glDisable(GL_CULL_FACE);
		//float quadratic[] =  { 1.0f, 0.0f, 0.00f };
		//.glPointParameterfvARB( GL_POint_DISTANCE_ATTENUATION_ARB, quadratic );
		//float maxSize = 0.0f;
		//glGetfloatv( GL_POint_SIZE_MAX_ARB, &maxSize );
		//if( maxSize > 100.0f )
		//	maxSize = 100.0f;
		//glPointSize( 300.0f );
		//.glPointParameterfARB( GL_POint_FADE_THRESHOLD_SIZE_ARB, 60.0f );
		//.glPointParameterfARB( GL_POint_SIZE_MIN_ARB, 1.0f );
		//.glPointParameterfARB( GL_POint_SIZE_MAX_ARB, maxSize );
		//glEnable( GL_POint_SPRITE_ARB );
		//glBegin( GL_POintS );
		//{
		//	glColor4fv( p->getColor().rgba() );

		//	( pos.x(), pos.y(), pos.z());
		//}
		//glEnd();
		//glDisable( GL_POint_SPRITE_ARB );
		//glEnable(GL_CULL_FACE);
		//glDisable(GL_BLEND);
		//glDisable(GL_TEXTURE_2D);
		//glDepthMask(true);
		pbit++;
	}
}

void ParticleEmitter::setAlive(bool alive)
{
	m_alive = alive;
	if (m_alive)
	{
		m_pfxManager->registerAliveEmitter(this);
	}
}

void ParticleEmitter::setPosition(const Vector3& position)
{
	m_position = position;
}

void ParticleEmitter::setStartVelocity(float startvelocity)
{
	m_startvelocity = startvelocity;
}