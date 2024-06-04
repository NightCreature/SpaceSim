#pragma once

#include "Core/Types/Types.h"

namespace VFS
{

enum class FileMode : uint8
{
    Open,
    Create,
    OpenAndCreate,
    OpenAndCreateTruncate
};

enum class FileOperation : uint8
{
    NoOp,
    Read,
    Write,
};

}