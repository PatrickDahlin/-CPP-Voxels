#ifndef _TIME_HPP_
#define _TIME_HPP_

#include <chrono>
#include <thread>
typedef std::chrono::high_resolution_clock Clock;


class Time {
friend class Game;
public:

	static double time_since_startup_sec();

private:

	static void set_start();

	static std::chrono::high_resolution_clock::time_point start_time;
};


#endif