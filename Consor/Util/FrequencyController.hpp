#ifndef FREQUENCY_CONTROLLER_H
#define FREQUENCY_CONTROLLER_H

#include <string>

namespace Consor
{
	namespace Util
	{
		class CFrequencyController
		{
			double _Next;
			double _Frequency;
			double _FequencyTime;
			double _Delta;
			double _LastThink;
			std::string _Name;
		public:
			CFrequencyController(const std::string& Name, double Frequency);

			void SetNextTime(double Time);
			void ThinkIn(double Seconds);
			double NextTime() const;
			bool Check();
			double Delta() const;
		};
	};
};
#endif