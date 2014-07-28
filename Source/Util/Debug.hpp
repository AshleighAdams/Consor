#ifndef CONSOR_UTIL_DEBUG_H
#define CONSOR_UTIL_DEBUG_H

#include <string>
#include "StringUtils.hpp"

#include <functional>

namespace Consor
{
	namespace Util
	{
		// FUCK YOU MICROSOFT
		//using LogCallback = std::function<void(const std::string&)>;
		typedef std::function<void(const std::string&)> LogCallback;

		extern void Log(const std::string& Message);
		/// The argument `callback` will be called when a new log message is generated.
		void HookLog(LogCallback callback);

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
