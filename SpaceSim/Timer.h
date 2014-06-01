#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

//-----------------------------------------------------------------------------
//! @brief  interface for timer classes
//! @remark
//-----------------------------------------------------------------------------
class ITimer
{
public:
    ITimer() : 
      m_resolution(0.0),
      m_time(0.0),
      m_baseTime(0.0),
      m_elapsedTime(0.0f)
    {}
    virtual ~ITimer() {}

    inline double getResolution() const { return m_resolution; }
    const double getTime() const { return m_time; }
    const float  getElapsedTime() const { return m_elapsedTime; }
    virtual void update() = 0;
protected:
    double m_resolution;
    double m_time;
    double m_baseTime;
    float  m_elapsedTime;
private:
};

//-----------------------------------------------------------------------------
//! @brief   High performance timer
//! @remark  Uses QueryPerformance timer to get the time
//-----------------------------------------------------------------------------
class PerformanceTimer : public ITimer
{
public:
    PerformanceTimer();
    ~PerformanceTimer() {}

    virtual void update();
protected:
private:
    void limitThreadToProc();
    void getNow();
    LARGE_INTEGER m_now;
    LARGE_INTEGER m_timePerformance;
    LARGE_INTEGER m_previousTime;
};