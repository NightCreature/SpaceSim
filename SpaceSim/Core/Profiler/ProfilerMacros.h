#pragma once

#ifdef PROFILING 
#include <Brofiler.h>
#endif

#ifdef PROFILING
#define PROFILE_EVENT(event_name, event_color) BROFILER_CATEGORY(event_name, Profiler::Color::event_color);
#else
#define PROFILE_EVENT(event_name, event_color) ;
#endif

#ifdef PROFILING
#define PROFILE_FRAME(frame_name) BROFILER_FRAME(frame_name);
#else
#define PROFILE_FRAME(frame_name) ;
#endif

#ifdef PROFILING
#define PROFILE_THREAD(thread_name) BROFILER_THREAD(thread_name);
#else
#define PROFILE_THREAD(thread_name) ;
#endif