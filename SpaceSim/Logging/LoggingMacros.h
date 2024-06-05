#pragma once

#include "Logging/LoggingFunctions.h"

//#define MSG_TRACE_WITH_FILE_LINENUMBER(severity, channel, msg, ...) debugOutput(severity, channel, __FILE__, __LINE__, msg, __VA_ARGS__);
#define MSG_TRACE_WITH_FILE_LINENUMBER(severity, channel, msg, ...) debugOutput(severity, channel, std::source_location::current(), msg, __VA_ARGS__);

#define MSG_TRACE_WITH_FILE_LINENUMBER_FMT(severity, channel, msg, ...) debugOutputFmt(severity, channel, std::source_location::current(), msg, __VA_ARGS__);


//#ifndef _DEBUG
//#define MSG_TRACE(message_string, ...) 
//#define MSG_TRACE_CHANNEL(channel, msg, ...) 
//#else
#define MSG_TRACE(message_string, ...) MSG_TRACE_WITH_FILE_LINENUMBER(TraceSeverity::EDEBUG, "", message_string, __VA_ARGS__);
#define MSG_TRACE_CHANNEL(channel, msg, ...) MSG_TRACE_WITH_FILE_LINENUMBER(TraceSeverity::EDEBUG, channel, msg, __VA_ARGS__);
#define MSG_TRACE_CHANNEL_FMT(channel, msg, ...) MSG_TRACE_WITH_FILE_LINENUMBER_FMT(TraceSeverity::EDEBUG, channel, msg, __VA_ARGS__);

#define MSG_WARN_CHANNEL(channel, msg, ...) MSG_TRACE_WITH_FILE_LINENUMBER(TraceSeverity::EWARN, channel, msg, __VA_ARGS__);
#define MSG_ERROR_CHANNEL(channel, msg, ...) MSG_TRACE_WITH_FILE_LINENUMBER(TraceSeverity::EERROR, channel, msg, __VA_ARGS__);
#define MSG_ASSERT_CHANNEL(channel, msg, ...) MSG_TRACE_WITH_FILE_LINENUMBER(TraceSeverity::EASSERT, channel, msg, __VA_ARGS__);
//#endif