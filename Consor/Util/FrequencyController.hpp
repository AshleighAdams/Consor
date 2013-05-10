#ifndef FREQUENCY_CONTROLLER_H
#define FREQUENCY_CONTROLLER_H

#include <string>

namespace Consor
{
	namespace Util
	{
		class CFrequencyController
		{
			double m_Next;
			double m_Frequency;
			double m_FequencyTime;
			double m_Delta;
			double m_LastThink;
			std::string m_Name;
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