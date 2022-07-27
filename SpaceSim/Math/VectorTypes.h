#pragma once

#include "Core/Types/Types.h"
#include "Math\VectorBySize.h"
#include <limits>

using Vector2i = VectorBySize<int, 2>;
using Vector3i = VectorBySize<int, 3>;
using Vector4i = VectorBySize<int, 4>;

using Vector2s = VectorBySize<size_t, 2>;
using Vector3s = VectorBySize<size_t, 3>;
using Vector4s = VectorBySize<size_t, 4>;

using Vector2i64 = VectorBySize<int64, 2>;
using Vector3i64 = VectorBySize<int64, 3>;
using Vector4i64 = VectorBySize<int64, 4>;

using Vector2d = VectorBySize<double, 2>;
using Vector3d = VectorBySize<double, 3>;
using Vector4d = VectorBySize<double, 4>;