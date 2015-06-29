#include "StringHelperFunctions.h"
#include <assert.h>

//#ifndef _DEBUG
//#define ASSERT(message_string, ...) 
//#define ASSERT(channel, msg, ...) 
//#else
#define ASSERT(condition, msg, ...) if (!(condition)) { MSG_TRACE_WITH_FILE_LINENUMBER(EASSERT, "", msg, __VA_ARGS__); assert((condition)); }
#define ASSERT_CHANNEL(condition, channel, msg, ...) if (!(condition)) { MSG_TRACE_WITH_FILE_LINENUMBER(EASSERT, channel, msg, __VA_ARGS__); assert((condition)); }
//#endif