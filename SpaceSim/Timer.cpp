#include "Timer.h"
#include <MMSystem.h>

//-----------------------------------------------------------------------------
//! @brief  Performance timer Constructor
//! @remark
//-----------------------------------------------------------------------------
PerformanceTimer::PerformanceTimer()
{
    LARGE_INTEGER frequency = {0};
    if (QueryPerformanceFrequency(&frequency))
    {
        m_resolution = (double)frequency.QuadPart;
    }
    limitThreadToProc();
}

//-----------------------------------------------------------------------------
//! @brief  Get the actual time
//! @remark
//-----------------------------------------------------------------------------
void PerformanceTimer::getNow()
{
    QueryPerformanceCounter(&m_now);
}

//-----------------------------------------------------------------------------
//! @brief  update the timer, gets triggered every update loop
//! @remark
//-----------------------------------------------------------------------------
void PerformanceTimer::update()
{
    getNow();
    m_previousTime = m_timePerformance;
    m_timePerformance = m_now;
    //Convert to floats
    m_time = (m_now.QuadPart) / m_resolution;
    m_elapsedTime = (float)((m_now.QuadPart - m_previousTime.QuadPart) / m_resolution);
    if (m_elapsedTime < 0.0f)
    {
        m_elapsedTime = 0.0f;
    }
}

//-----------------------------------------------------------------------------
//! @brief  limit the timer to calling thread
//! @remark This function should be called after construction
//-----------------------------------------------------------------------------
void PerformanceTimer::limitThreadToProc()
{
    HANDLE hCurrentProcess = GetCurrentProcess();

    // Get the processor affinity mask for this process
    DWORD_PTR dwProcessAffinityMask = 0;
    DWORD_PTR dwSystemAffinityMask = 0;

    if( GetProcessAffinityMask( hCurrentProcess, &dwProcessAffinityMask, &dwSystemAffinityMask ) != 0 &&
        dwProcessAffinityMask )
    {
        // Find the lowest processor that our process is allows to run against
        DWORD_PTR dwAffinityMask = ( dwProcessAffinityMask & ( ( ~dwProcessAffinityMask ) + 1 ) );

        // Set this as the processor that our thread must always run against
        // This must be a subset of the process affinity mask
        HANDLE hCurrentThread = GetCurrentThread();
        if( INVALID_HANDLE_VALUE != hCurrentThread )
        {
            SetThreadAffinityMask( hCurrentThread, dwAffinityMask );
            CloseHandle( hCurrentThread );
        }
    }

    CloseHandle( hCurrentProcess );
}