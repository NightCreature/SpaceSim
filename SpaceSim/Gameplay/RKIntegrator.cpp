#include "RKIntegrator.h"


RKIntegrator::RKIntegrator(void)
{
}


RKIntegrator::~RKIntegrator(void)
{
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
State RKIntegrator::integrate( const State& initial, float t, float dt )
{
    Derivative a,b,c,d;
    State state = initial;

    a = evaluate( state, t, 0.0f, Derivative() );
    b = evaluate( state, t, dt*0.5f, a );
    c = evaluate( state, t, dt*0.5f, b );
    d = evaluate( state, t, dt, c );

    float oneSixth = 1.0f / 6.0f;
    float dxdt = oneSixth * ( a.dx + 2.0f*(b.dx + c.dx) + d.dx );
    float dvdt = oneSixth * ( a.dv + 2.0f*(b.dv + c.dv) + d.dv );

    state.m_position = state.m_position + dxdt * dt;
    state.m_velocity = state.m_velocity + dvdt * dt;

    return state;
}

//-----------------------------------------------------------------------------
//! @brief   TODO enter a description
//! @remark
//-----------------------------------------------------------------------------
Derivative RKIntegrator::evaluate( const State& initial, float t, float dt, const Derivative& d )
{
    State state;
    state.m_position = initial.m_position + d.dx*dt;
    state.m_velocity = initial.m_velocity + d.dv*dt;

    Derivative output;
    output.dx = state.m_velocity;
    output.dv = m_derivativeFunction(t+dt, state);
    return output;
}
