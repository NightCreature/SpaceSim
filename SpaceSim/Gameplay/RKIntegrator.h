#pragma once

#include <functional>

struct State
{
    float m_position;
    float m_velocity;
};

struct Derivative
{
    float dx;      // dx/dt = velocity
    float dv;      // dv/dt = acceleration
};

class RKIntegrator
{
public:
    RKIntegrator(void);
    ~RKIntegrator(void);

    typedef std::function<float(float, State)> DerivativeFunction;

    void setDerivativeFunction(const DerivativeFunction& function) { m_derivativeFunction = function; }
    
    State integrate(const State& initial, float t, float dt);
private:
    Derivative evaluate( const State& initial, float t, float dt, const Derivative& d );

    DerivativeFunction m_derivativeFunction;
};

