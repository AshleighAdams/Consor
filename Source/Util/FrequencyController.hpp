#ifndef FREQUENCY_CONTROLLER_H
#define FREQUENCY_CONTROLLER_H

#include <string>

namespace Consor
{
	namespace Util
	{
		/// Run a task in a timely fashion.
		class FrequencyController
		{
			double _Next;
			double _Frequency;
			double _FequencyTime;
			double _Delta;
			double _LastThink;
			std::string _Name;
		public:
			/// Constructs the controller.
			/// If `Check()` is called too slowly, it skips a cycle and sends a message to the log.
			/// \param Name The name used to outputting to the log in case it executes slowly.
			/// \param Frequency How often to run, in Hz.
			FrequencyController(const std::string& Name, double Frequency);

			/// Set the next time `Check()` will return true.
			void SetNextTime(double Time);
			/// Equivalent to `SetNextTime(GetTime() + Seconds)`.
			void ThinkIn(double Seconds);
			/// \return The time next scheduled to execute next.
			double NextTime() const;
			/// Check if the task is ready to execute.
			/// If it returns true, then the value `NextTime()` returns is increased by `Delta()`
			bool Check();
			/// Returns the 
			double Delta() const;
		};
	};
};
#endif
