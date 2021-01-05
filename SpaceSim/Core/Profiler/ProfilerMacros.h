#pragma once

//#define PROFILING

#ifdef PROFILING 
#include "Core/Profiler/Profiler.h"
#endif

#ifdef PROFILING 
#define PROFILE_CONCAT_IMPL(a, b) a##b
#define PROFILE_CONCAT(a, b) PROFILE_CONCAT_IMPL(a, b)

#define PROFILE_EVENT_INTERNAL(name, file, function, line) \
static size_t PROFILE_CONCAT(eventDesc_, line) = Profiling::Profiler::GetInstance().RegisterProfilingEventDescription(Profiling::EventDescriptor(name, file, function, line)); \
Profiling::EventWrapper PROFILE_CONCAT(event_, line)( PROFILE_CONCAT(eventDesc_, line) );

#define PROFILE_EVENT2(name) PROFILE_EVENT_INTERNAL(name, __FILE__, __FUNCTION__, __LINE__)
#endif

#ifdef PROFILING
#define PROFILE_EVENT(event_name, event_color) PROFILE_EVENT2(event_name)
#else
#define PROFILE_EVENT(event_name, event_color) ;
#endif

#ifdef PROFILING
#define PROFILE_FRAME(frame_name) Profiling::Profiler::GetInstance().RegisterFrameName(frame_name);//BROFILER_FRAME(frame_name); 
#else
#define PROFILE_FRAME(frame_name) ;
#endif

#ifdef PROFILING
#define PROFILE_THREAD(thread_name) Profiling::Profiler::GetInstance().RegisterThreadName(GetCurrentThreadId(), thread_name);// BROFILER_THREAD(thread_name);
#else
#define PROFILE_THREAD(thread_name) ;
#endif