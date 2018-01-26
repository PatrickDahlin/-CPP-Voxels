#include "../headers/core/Thread.hpp"
#include "../headers/core/Macros.hpp"
#include <SDL2/SDL.h>

static int run(void* data)
{
	Thread* t = (Thread*)data;

	/*std::string s = "Started a new thread: ";
	long time = SDL_GetTicks();
	s += std::to_string(time);
	s += "\n";

	t->print(s);

	SDL_Delay(1000);

	s = "Thread finished: ";
	time = SDL_GetTicks();
	s += std::to_string(time);
	s += "\n";

	//t->print(s);*/
	
	t->run_internal();

	return 0;
}

Thread::Thread()
{
	thread = SDL_CreateThread(run, "Game Thread", (void*)this);

	if(thread == NULL)
	{
		error("Failed to create thread!\n");
		return;
	}
}

void Thread::print(std::string msg)
{
	cout(msg);
}

void Thread::join()
{
	running = false;
}