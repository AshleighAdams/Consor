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

		template<typename T>
		T Scale(T Value, T FromMin, T FromMax, T ToMax, T ToMin)
		{
			T from_diff = FromMax - FromMin;
			T to_diff = ToMax - ToMin;

			T y = (Value - FromMin) / from_diff;
			return ToMin + to_diff * y;
		}
	}
}

#endif