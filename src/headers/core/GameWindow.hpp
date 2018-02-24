#ifndef _GAMEWINDOW_HPP_
#define _GAMEWINDOW_HPP_
// We want pointers
#pragma GCC diagnostic ignored "-Weffc++"

#include "core/Common.hpp"
#include <GL/glew.h>
#include <SDL2/SDL.h>

class Game;

struct WindowSettings {
/*public:
	WindowSettings(const char* title, 
					int width, 
					int height, 
					int refresh_rate, 
					int display_index, 
					bool fullscreen,
					bool borderless,
					bool vsync,
					bool is_vulkan) : 
						title(title),
						width(width),height(height),
						refresh_rate(refresh_rate),
						display_index(display_index),
						fullscreen(fullscreen),
						borderless(borderless),
						vsync(vsync),
						vulkan_renderer(is_vulkan)
	{
			assert(width > 0  && height > 0);
			assert(refresh_rate >= 0 && refresh_rate < 9999);
			assert(display_index >= 0 && display_index < SDL_GetNumVideoDisplays());
	}
	~WindowSettings(){}

	int get_width() const { return width; }
	int get_height() const { return height; }
	int get_refresh_rate() const { return refresh_rate; }
	int get_display_index() const { return display_index; }
	bool get_vsync() const { return vsync; }
	bool is_borderless() const { return borderless; }
	bool is_fullscreen() const { return fullscreen; }
	bool is_vulkan() const { return vulkan_renderer; }
	const char* get_title() const { return title; }

private://*/

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

	WindowSettings get_settings();

protected:

	void create_window();

	void setup_gl_context();

	SDL_Window*		window;
	SDL_GLContext 	context;

	WindowSettings 	settings;

	Game*			game;
	
};

#endif