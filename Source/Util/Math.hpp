#ifndef UTIL_MATH_H
#define UTIL_MATH_H

#include <cmath>

namespace Consor
{
	namespace Util
	{
		/// Round the value to the specified percision.
		/// \param Value The number to round.
		/// \param Precision The smallest unit possible.  Example: 0.25, then 1.3 is rounded to 1.25, 1.9 is rounded to 2.0.
		template<typename T> 
		T Round(T Value, T Precision)
		{
			Precision = T(1) / Precision;
			return floor(Value * Precision + T(0.5)) / Precision;
		}

		/// Scale or map a number from one range to another.
		/// \param Value The input value.
		/// \param FromMin The minimum input value.
		/// \param FromMax The maximum input value.
		/// \param ToMin The input minimum will map to this.
		/// \param ToMax The input maximum will map to this.
		/// \return `Value` mapped to the from-range to the to-range.
		/// \warning The input value can go out of bounds, and will be extrapolated.
		/// \warning The range minimium does not have to be less than the range maximium.
		template<typename T>
		T Map(T Value, T FromMin, T FromMax, T ToMin, T ToMax)
		{
			T from_diff = FromMax - FromMin;
			T to_diff = ToMax - ToMin;

			T y = (Value - FromMin) / from_diff;
			return ToMin + to_diff * y;
		}
		
		/// \deprecated Moved to `Consor::Util::Map()`
		template<typename T>
		T Scale(T Value, T FromMin, T FromMax, T ToMin, T ToMax)
		{
			return Map<T>(Value, FromMin, FromMax, ToMin, ToMax);
		}
	}
}

#endif
