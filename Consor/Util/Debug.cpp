#include "Debug.hpp"
#include "Time.hpp"

#include <iostream>

using namespace std;

void Consor::Util::Log(const string& Message)
{
	double time = Util::GetTime();
	time = ((double)(int)(time * 1000) / 1000.0);
	string fin = Util::FormatString("[%] %", time, Message);

	cerr << fin << "\n";
}