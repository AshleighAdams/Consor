#ifndef UTIL_TIME_H
#define UTIL_TIME_H

namespace Consor
{
	namespace Util
	{
		/// Returns the number of seconds since the program has been started.
		extern double GetTime();
		/// Sleep for the specified duration.
		/// \param Duration Number of seconds to sleep.
		extern void Sleep(double Duration);
	}
}

#endif
