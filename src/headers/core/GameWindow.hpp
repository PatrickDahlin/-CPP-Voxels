#ifndef _GAMEWINDOW_HPP_
#define _GAMEWINDOW_HPP_
// We want pointers
#pragma GCC diagnostic ignored "-Weffc++"

#include "core/Common.hpp"
#include <GL/glew.h>
#include <SDL2/SDL.h>

class Game;

struct WindowSettings {

	const char* title;
	// This is framebuffer size
	int width;
	int height;

	int refresh_rate;
	int display_index;

	bool fullscreen;
	bool borderless;

	// In case of fullscreen & borderless we need to
	// set window to native monitor size and keep fbo
	// size as requested

	bool vsync; // Late-swap ignored here
	bool vulkan_renderer; // OpenGL otherwise
};

// @TODO add support for changing displaymodes with different refreshrates

class GameWindow 
{
public:
	GameWindow(WindowSettings settings);
	~GameWindow();

	void swap_buffers();

	void attach_game(Game* game);

	void set_mouse_pos(int x, int y);

	void apply_settings(WindowSettings new_settings);

	void destroy();

	int get_width();

	int get_height();

	// Since borderless technically is native size but we
	// render at a lower res, we'll return the real window size here

	int get_real_screen_width();

	int get_real_screen_height();

	WindowSettings get_settings();

protected:

	void create_window();

	void setup_gl_context();

	SDL_Window*		window;
	SDL_GLContext 	context;

	WindowSettings 	settings;

	// These aren't window options, thus we keep them here
	int real_width;
	int real_height;

	Game*			game;
	
};

#endif