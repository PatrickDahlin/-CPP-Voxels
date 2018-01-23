#ifndef _GAMEWINDOW_HPP_
#define _GAMEWINDOW_HPP_
// We want pointers
#pragma GCC diagnostic ignored "-Weffc++"

#include <GL/glew.h>
#include <SDL2/SDL.h>


class GameWindow 
{
public:
	GameWindow(const char* title, int width, int height, SDL_WindowFlags flags);
	~GameWindow();

	/// This sets all windowflags for window
	/// SDL_WINDOW_OPENGL is needed!!
	void set_window_flags(SDL_WindowFlags flags);

	void swap_buffers();

	void set_vsync(bool vsync);

	void destroy();

protected:

	void create_window();

	void setup_gl_context();

	SDL_Window*		window;
	SDL_GLContext 	context;
	SDL_WindowFlags	flags;

	const char*		title;
	int				width, height;
	bool			vsync;

};

#endif