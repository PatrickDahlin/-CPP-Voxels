#ifndef _THREAD_HPP_
#define _THREAD_HPP_
#include <string>

class SDL_Thread;

class Thread {
public:
	Thread();
	~Thread();

	// Print message to main thread output stream
	void print(std::string msg);

	// Request thread to close
	void join();

	// DO NOT CALL THIS MANUALLY
	// Override in child class as worker method
	virtual void run_internal() = 0;

protected:

	SDL_Thread*		thread;
	bool			running = true;

};

#endif