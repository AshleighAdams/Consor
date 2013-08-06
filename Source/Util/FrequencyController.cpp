#include "FrequencyController.hpp"
#include "Time.hpp"

using namespace std;
using namespace Consor;
using namespace Consor::Util;

FrequencyController::FrequencyController(const string& Name, double Frequency)
{
	_Frequency = Frequency;
	_FequencyTime = 1.0 / _Frequency;
	_Next = Util::GetTime() + _FequencyTime;
	_Delta = 0;
	_Name = Name;
}

void FrequencyController::SetNextTime(double Time)
{
	_Next = Time;
}
void FrequencyController::ThinkIn(double Seconds)
{
	SetNextTime(Util::GetTime() + Seconds);
}

double FrequencyController::NextTime() const
{
	return _Next;
}

bool FrequencyController::Check()
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

double FrequencyController::Delta() const
{
	return _Delta;
}
