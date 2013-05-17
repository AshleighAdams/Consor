#include "Util/FrequencyController.hpp"
#include "Util/Time.hpp"

using namespace std;
using namespace Consor;
using namespace Consor::Util;

CFrequencyController::CFrequencyController(const string& Name, double Frequency)
{
	_Frequency = Frequency;
	_FequencyTime = 1.0 / _Frequency;
	_Next = Util::GetTime() + _FequencyTime;
	_Delta = 0;
	_Name = Name;
}

void CFrequencyController::SetNextTime(double Time)
{
	_Next = Time;
}
void CFrequencyController::ThinkIn(double Seconds)
{
	SetNextTime(Util::GetTime() + Seconds);
}

double CFrequencyController::NextTime() const
{
	return _Next;
}

bool CFrequencyController::Check()
{
	if(Util::GetTime() < _Next)
		return false;

	_Next += _FequencyTime;

	if(_Next < Util::GetTime()) // timeout
	{
		ThinkIn(_FequencyTime);
	}


	_Delta = Util::GetTime() - _LastThink;
	_LastThink = Util::GetTime();
	return true;
}

double CFrequencyController::Delta() const
{
	return _Delta;
}
