#include "Util/FrequencyController.hpp"
#include "Util/Time.hpp"

using namespace std;
using namespace Consor;
using namespace Consor::Util;

CFrequencyController::CFrequencyController(const string& Name, double Frequency)
{
	m_Frequency = Frequency;
	m_FequencyTime = 1.0 / m_Frequency;
	m_Next = Util::GetTime() + m_FequencyTime;
	m_Delta = 0;
	m_Name = Name;
}

void CFrequencyController::SetNextTime(double Time)
{
	m_Next = Time;
}
void CFrequencyController::ThinkIn(double Seconds)
{
	SetNextTime(Util::GetTime() + Seconds);
}

double CFrequencyController::NextTime() const
{
	return m_Next;
}

bool CFrequencyController::Check()
{
	if(Util::GetTime() < m_Next)
		return false;

	m_Next += m_FequencyTime;

	if(m_Next < Util::GetTime()) // timeout
	{
		ThinkIn(m_FequencyTime);
	}


	m_Delta = Util::GetTime() - m_LastThink;
	m_LastThink = Util::GetTime();
	return true;
}

double CFrequencyController::Delta() const
{
	return m_Delta;
}
