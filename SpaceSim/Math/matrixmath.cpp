#include "matrixmath.h"
#include <cmath>

namespace math
{
	float toRadian(float degree)
	{
		return degree * (gmPI / 180);
	}

	float toDegree(float radian)
	{
		return radian * (180 / gmPI);
	}
}
