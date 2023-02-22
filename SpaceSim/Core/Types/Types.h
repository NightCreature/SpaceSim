#pragma once

//#include "UI/Messages.h"
//#include "Core/Resource/Resourceable.h"
//#include "Core/Resource/GameResource.h"
//#include "Core/Resource/RenderResource.h"
#include <cstddef>
#include <cstdint>
#include <vector>

using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

using uint = uint32; //Shaders

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

using byte = uint8;
using word = uint16;
using dword = uint32;
using qword = uint64;

class RenderInstance;

typedef std::vector<RenderInstance*> RenderInstanceTree;