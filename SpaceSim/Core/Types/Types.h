#pragma once

#include "UI/Messages.h"
#include "Core/Resource/Resourceable.h"
#include "Core/Resource/GameResource.h"
#include "Core/Resource/RenderResource.h"
#include <cstddef>
#include <vector>

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

class RenderInstance;

typedef std::vector<RenderInstance*> RenderInstanceTree;