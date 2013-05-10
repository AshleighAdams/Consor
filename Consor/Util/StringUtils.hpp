#ifndef UTIL_STRINGUTIL_H
#define UTIL_STRINGUTIL_H

#include <string>
#include <sstream>

namespace Consor
{
	namespace Util
	{
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
		inline void FormatString(std::stringstream& ss, const char* pFormat, T Value, Args... args)
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

		template<class... Args>
		inline std::string FormatString(const char* pFormat, Args... args)
		{
			std::stringstream ss;
			FormatString(ss, pFormat, args...);
			return ss.str();
		}

		extern std::string FormatTimeSpan(double Time);
		extern std::string WrapText(const std::string& input, size_t width, size_t* pOutWidth, size_t* pOutHeight);
	}
}

#endif