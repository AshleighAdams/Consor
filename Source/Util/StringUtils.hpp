#ifndef UTIL_STRINGUTIL_H
#define UTIL_STRINGUTIL_H

#include <string>
#include <sstream>
#include <stdexcept>
#include <limits>
#include <cmath>

namespace Consor
{
	namespace Util
	{
		/// \cond PRIVATE
		inline void FormatString(std::stringstream& ss, const char* pFormat)
		{
			while(*pFormat)
			{
				if(*pFormat == '%')
				{
					if(*(pFormat + 1) == '%')
						pFormat++;
					else
						throw std::invalid_argument("Format string is missing arguments!");
				}

				ss << *pFormat++;
			}
		}
		template<class T, class... Args>
		inline void FormatString(std::stringstream& ss, const char* pFormat, T&& Value, Args&&... args)
		{
			while(*pFormat)
			{
				if(*pFormat == '%')
				{
					if(*(pFormat + 1) == '%')
						pFormat++;
					else
					{
						ss << Value;
						FormatString(ss, pFormat + 1, args...);
						return;
					}
				}
				ss << *pFormat++;
			}

			throw std::invalid_argument("Too many arguments provided!");
		}
		/// \endcond
		
		/// Format a string, use % to represent argument, and %% to represent "%".
		/// \param pFormat The input format string.
		/// \param args The arguments used to format the string.
		/// \return The formatted string.
		/// \exception std::invalid_argument Incorrect number of arguments.
		/// \note Requires `std::ostream& operator<<(std::ostream&, const T&)` to be defined, where `T` is the type of the argument.
		/// \par Example
		/// \include format-string.cpp
		/// \include format-string.txt
		template<class... Args>
		inline std::string FormatString(const char* pFormat, Args&&... args)
		{
			std::stringstream ss;
			FormatString(ss, pFormat, args...);
			return ss.str();
		}

		/// Format the input seconds into a reasonable string.  Will attempt to cut at word breaks, else hyphenate.
		extern std::string FormatTimeSpan(double Time);
		/// Wrap a string around a specified width.
		/// \param input The input string.
		/// \param width The target width.
		/// \param[out] pOutWidth If not nullptr, it's set to the real width of the string.
		/// \param[out] pOutHeight If not nullptr, it's set to the real height of the string.
		/// \return The new width-constrained string.
		extern std::string WrapText(const std::string& input, size_t width, size_t* pOutWidth, size_t* pOutHeight);
	}
}

#endif
