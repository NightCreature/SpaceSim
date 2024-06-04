#pragma once

#include "Core/Types/Types.h"
#include "Math\VectorBySize.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
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

//template<class T, class Archive>
//constexpr bool GlobalHasCreate = requires (T value, Archive ar) { Serialise(ar, value); };
//
////Allows us to add global functions to do serialisation and extend types
//template<typename T, typename Archive>
//concept HasGlobalSerialiseWrite = requires(T value, Archive archive)
//{
//    Serialise(archive, value, std::false_type());
//};