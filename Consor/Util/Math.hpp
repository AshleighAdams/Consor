#ifndef UTIL_MATH_H
#define UTIL_MATH_H

#include <cmath>

namespace Consor
{
	namespace Util
	{
		template<typename T> 
		T Round(T Value, T Precision)
		{
			Precision = T(1) / Precision;
			return floor(Value * Precision + T(0.5)) / Precision;
		}
	}
}

#endif