#include "Time.hpp"

#include <chrono>
#include <ctime>
#include <thread>

using namespace std;
using namespace std::chrono;

double Consor::Util::GetTime()
{
	static high_resolution_clock::time_point start = high_resolution_clock::now();
	high_resolution_clock::time_point now = high_resolution_clock::now();

	duration<double> time_span = duration_cast<duration<double>>(now - start);
			
	return time_span.count();
}

void Consor::Util::Sleep(double Duration)
{
	// seconds * mili-seconds * micro-seconds * nano-seconds
	nanoseconds dura((long long)(Duration * 1000.0 * 1000.0 * 1000.0));
	this_thread::sleep_for(dura);
}