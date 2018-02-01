#include "core/Thread.hpp"
#include "core/Macros.hpp"
#include <SDL2/SDL.h>

static int run(void* data)
{
	Thread* t = (Thread*)data;
	
	cout("Hai from thread");

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

void Thread::join()
{
	running = false;
}