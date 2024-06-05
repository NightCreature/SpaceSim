#pragma once

//#define PROFILING

#include "Core/Macros/PlatformMacros.h"

#include "microprofile.h"
#include "Optick.h"

#ifdef PROFILING 
#include "Core/Profiler/Profiler.h"
#endif

#include <source_location>
#include <windows.h>
#include <Pix3.h>

//Incorporate https://devblogs.microsoft.com/pix/winpixeventruntime/ for PIX support

#ifdef PROFILING 
#define PROFILE_CONCAT_IMPL(a, b) a##b
#define PROFILE_CONCAT(a, b) PROFILE_CONCAT_IMPL(a, b)

#define PROFILE_EVENT_INTERNAL(name, file, function, line) \
static size_t PROFILE_CONCAT(eventDesc_, line) = Profiling::Profiler::GetInstance().RegisterProfilingEventDescription(Profiling::EventDescriptor(name, file, function, line)); \
Profiling::EventWrapper PROFILE_CONCAT(event_, line)( PROFILE_CONCAT(eventDesc_, line) );

#define PROFILE_EVENT2(name) PROFILE_EVENT_INTERNAL(name, __FILE__, __FUNCTION__, __LINE__)
#endif

#define PROFILE_EVENT_NORMAL(group, event_name, event_color)\
MICROPROFILE_SCOPEI(group, event_name, event_color); \
OPTICK_EVENT(event_name, OPTICK_MAKE_CATEGORY(Optick::Filter::None, event_color))\
PIXScopedEvent(event_color, event_name)

#define PROFILE_FRAME2(name)\
OPTICK_FRAME(name);

#ifdef PROFILING
#define PROFILE_EVENT(group, event_name, event_color)\
PROFILE_EVENT2(event_name);\
PROFILE_EVENT_NORMAL(group, event_name, event_color);
#else
#define PROFILE_EVENT(group, event_name, event_color) PROFILE_EVENT_NORMAL(group, event_name, event_color);
#endif


#ifdef PROFILING
#define PROFILE_FUNCTION()\
    std::source_location location = std::source_location::current();\
    PROFILE_EVENT2(location.function_name());\
    PROFILE_EVENT_NORMAL(location.file_name(), location.function_name(), 0);
#else
#define PROFILE_FUNCTION()\
    std::source_location location = std::source_location::current();\
    PROFILE_EVENT_NORMAL(location.file_name(), location.function_name(), 0x00FF);
#endif


#define PROFILE_TAG(tag_name) \
    std::source_location location = std::source_location::current();\
    PROFILE_EVENT(location.file_name(), tag_name, 0x00FF)



#ifdef PROFILING
#define PROFILE_FRAME(frame_name) Profiling::Profiler::GetInstance().RegisterFrameName(frame_name); PROFILE_FRAME2(frame_name);
#else
#define PROFILE_FRAME(frame_name) PROFILE_FRAME2(frame_name);
#endif

#define PROFILE_GPU_FLIP(swapchain, rendercontext)\
OPTICK_GPU_FLIP(swapchain);\
MicroProfileFlip(rendercontext);

#define PROFILE_THREAD2(thread_name) \
OPTICK_THREAD(thread_name); \
MicroProfileOnThreadCreate(thread_name);

#ifdef PROFILING
#define PROFILE_THREAD(thread_name) Profiling::Profiler::GetInstance().RegisterThreadName(GetCurrentThreadId(), thread_name); PROFILE_THREAD2(thread_name);
#else
#define PROFILE_THREAD(thread_name) PROFILE_THREAD2(thread_name);
#endif

//#define PROFILE_GPU_EVENT(event_name, commandlist)\
//PIXScopedEvent()
//
//#ifdef PROFILING
//#define PROFILING_GPU_EVENT(event_name)
//    
//#else
//#define PROFILING_GPU_EVENT(event_name)
//#endif