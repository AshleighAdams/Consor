#ifndef CONSOR_UTIL_DEBUG_H
#define CONSOR_UTIL_DEBUG_H

#include <string>
#include "StringUtils.hpp"

namespace Consor
{
	namespace Util
	{
		extern void Log(const std::string& Message);

		template<class... Args>
		inline void Log(const char* pFormat, Args... args)
		{
			std::stringstream ss;
			FormatString(ss, pFormat, args...);

			std::string x = ss.str();
			Log(x);
		}
	};
};

#endif