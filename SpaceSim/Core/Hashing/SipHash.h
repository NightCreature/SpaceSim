#pragma once

#include "Core/StringOperations/StringHelperFunctions.h"
#include "Core/Types/Types.h"

namespace SipHash
{
uint64 hashString(const char* strData, size_t size);
uint64 hashBinaryData(const byte* data, size_t size);

}


