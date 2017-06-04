#ifndef PARTICLE_H
#define PARTICLE_H

#include "Math/vector3.h"
#include "Graphics/color.h"
#include "Graphics/interpolatecurveRGBA.h"

namespace ParticleSystem
{

class Particle
{
public:
    enum State {
        Unborn,
        Living,
        Dead
    };

    Particle();
    ~Particle();

    void initialize(const Vector3& position, const Vector3& velocity, const Color& startcolor, float lifetime);
    void initializeColorCurve(Color& color1, Color& color2, Color& color3, Color& color4, float keyframe1, float keyframe2);
    void proceed(float elapsedtime);
    const Vector3 getPosition() const { return m_position; }
    const Vector3 getVelocity() const { return m_velocity; }
    void setLifeTime(float time) { m_lifetime = time; }
    float getLifeTime() const { return m_lifetime; }
    void setState(State state) { m_state = state; }
    State getState() const { return m_state; }
    float getSize() const { return m_size; }
    void setSize(float size) { m_size = size; }
    Color getStartColor() const { return m_startcolor; }
    Color getEndColor() const { return m_endcolor; }
    Color getColor() const { return m_intercolor; }
    void setStartColor(const Color& c) { m_startcolor = c; }
    void setEndColor(const Color& c) { m_endcolor = c; }
protected:
private:
    float getRelativeAge() const;

    interpolateCurveRGBA m_icrgba;
    Vector3 m_position;
    Vector3 m_velocity;//Velocity and direction
    Color   m_intercolor;
    Color	m_startcolor;
    Color	m_endcolor;
    State	m_state;
    float	m_lifetime;
    float	m_living;
    float	m_rotation;
    float	m_size;
};

}
#endif