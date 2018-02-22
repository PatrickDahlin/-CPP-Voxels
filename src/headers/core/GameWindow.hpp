#ifndef _GAMEWINDOW_HPP_
#define _GAMEWINDOW_HPP_
// We want pointers
#pragma GCC diagnostic ignored "-Weffc++"

#include "core/Common.hpp"
#include <GL/glew.h>
#include <SDL2/SDL.h>

struct WindowSettings {
	const char* title;
	int width;
	int height;
	bool fullscreen;
	bool borderless_fullscreen;
	bool vsync;
	bool is_vulkan; // OpenGL otherwise
};

class GameWindow 
{
public:
	GameWindow(WindowSettings settings);
	~GameWindow();

	void swap_buffers();

	void set_vsync(bool vsync);

	void set_fullscreen(bool fullscreen);

	void set_window_size(int w, int h);

	void set_mouse_pos(int x, int y);

	void destroy();

	int get_width();

	int get_height();

	bool is_fullscreen();

	WindowSettings get_settings();

protected:

	void create_window();

	void setup_gl_context();

	SDL_Window*		window;
	SDL_GLContext 	context;

	WindowSettings 	settings;
	
};

#endif