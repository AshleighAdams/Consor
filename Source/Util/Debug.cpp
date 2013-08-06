#include "Debug.hpp"
#include "Time.hpp"

#include <iostream>
#include <list>

using namespace std;
using namespace Consor;
using namespace Consor::Util;

std::list<LogCallback> _Subscribed;

void Util::Log(const string& Message)
{
	double time = Util::GetTime();
	time = ((double)(int)(time * 1000) / 1000.0);
	string fin = Util::FormatString("[%] %", time, Message);

	cerr << fin << "\n";

	for(LogCallback cb : _Subscribed)
		cb(fin);
}

void Util::HookLog(LogCallback callback)
{
	_Subscribed.push_back(callback);
}