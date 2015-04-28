#include "StringHelperFunctions.h"
#include <assert.h>

//#ifndef _DEBUG
//#define ASSERT(message_string, ...) 
//#define ASSERT(channel, msg, ...) 
//#else
#define ASSERT(condition, msg, ...) if (!condition) { MSG_TRACE_CHANNEL("ASSERT", msg, __VA_ARGS__); assert((condition)); }
//#endif