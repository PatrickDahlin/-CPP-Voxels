#include "core/Time.hpp"

std::chrono::high_resolution_clock::time_point Time::start_time = Clock::now();


void Time::set_start()
{
	start_time = Clock::now();
}

double Time::time_since_startup_sec()
{
	using std::chrono::nanoseconds;
	using std::chrono::duration_cast;

	auto duration = Clock::now() - start_time;
	
	auto a = duration_cast<nanoseconds>(duration);

	double out = (double)a.count() / 1000000000.0;
	return out;
}