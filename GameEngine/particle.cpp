#include "particle.h"
#include "vector4.h"
#include <iostream>

Particle::Particle()
{
}

Particle::~Particle()
{
}

void Particle::initialize(const Vector3& position, const Vector3& velocity, const Color& startcolor, float lifetime)
{
	m_state = Unborn;
	m_position = position;
	m_velocity = velocity;
	m_startcolor = startcolor;
	m_intercolor = startcolor;
	m_lifetime = lifetime;
	m_living = 0.0f;
}

void Particle::initializeColorCurve(Color& color1, Color& color2, Color& color3, Color& color4, float keyframe1, float keyframe2)
{
	Vector4 v1(color1.rgba()[0], color1.rgba()[1], color1.rgba()[2], color1.rgba()[3]),
			v2(color2.rgba()[0], color2.rgba()[1], color2.rgba()[2], color2.rgba()[3]),
			v3(color3.rgba()[0], color3.rgba()[1], color3.rgba()[2], color3.rgba()[3]),
			v4(color4.rgba()[0], color4.rgba()[1], color4.rgba()[2], color4.rgba()[3]);
	m_icrgba = interpolateCurveRGBA(v1, v2, v3, v4, keyframe1, keyframe2);
}

void Particle::proceed(float elapsedtime)
{
	static Vector3 windVelocity;
	static Vector3 finalVelocity;

	switch (m_state)
	{
	case Unborn:
		if (m_living <= m_lifetime)
		{
			m_state = Living;
		}
		break;

	case Living:
		if (m_living >= m_lifetime)
		{
			m_state = Dead;
		}
		else
		{
			m_living += elapsedtime;
			float relage = getRelativeAge();			
			Vector4 interpolated = m_icrgba.GetValue(relage);
			m_intercolor = Color(interpolated.x(), interpolated.y(), interpolated.z(), interpolated.w());

			// update velocity
			m_velocity += m_velocity * elapsedtime;
			m_position = m_position + m_velocity * elapsedtime;
		}
		break;

	default:
		break;
	}
}

float Particle::getRelativeAge() const
{
	return m_living / m_lifetime;
}
