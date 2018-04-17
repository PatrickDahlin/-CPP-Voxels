#ifndef _THREAD_HPP_
#define _THREAD_HPP_
#include <string>

class SDL_Thread;

// !!! No cleanup of threads!!

class Thread {
public:
	Thread();
	~Thread();

	// Request thread to close
	void join();

	// DO NOT CALL THIS MANUALLY
	// Override in child class as worker method
	virtual void run_internal() = 0;

protected:

	SDL_Thread*		thread;
	// Internal run can check this var to see if it needs to shut down
	bool			running = true;

};

#endif