#include "core/GameWindow.hpp"
#include <iostream>
#include <stdio.h>
#include "game/Game.hpp"

GameWindow::GameWindow(WindowSettings settings) :
	window(NULL), 
	context(NULL), 
	settings(settings), 
	real_width(settings.width), 
	real_height(settings.height), 
	game(nullptr)
{
	create_window();
}

GameWindow::~GameWindow()
{
	destroy();
}

void GameWindow::attach_game(Game* game)
{
	this->game = game;
}

void GameWindow::destroy()
{
	if(window)
	{
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(window);
	}

	window = nullptr;
}

void GameWindow::create_window()
{
	SDL_ClearError();

	int flags = ( settings.vulkan_renderer ? SDL_WINDOW_VULKAN : SDL_WINDOW_OPENGL );

	printf("Setting up window with settings:\n");
	printf("\t%ix%i\n",settings.width,settings.height);
	
	if(flags & SDL_WINDOW_FULLSCREEN)
		printf("\t- Fullscreen\n");
	if(flags & SDL_WINDOW_BORDERLESS)
		printf("\t- Borderless\n");
	if(flags & SDL_WINDOW_OPENGL)
		printf("\t- OpenGL\n");
	if(flags & SDL_WINDOW_FULLSCREEN_DESKTOP)
		printf("\t- Fullscreen-Desktop\n");
	if(flags & SDL_WINDOW_INPUT_GRABBED)
		printf("\t- Input-grabbed\n");
	if(flags & SDL_WINDOW_ALLOW_HIGHDPI)
		printf("\t- Allow-High-DPI\n");
	if(flags & SDL_WINDOW_ALWAYS_ON_TOP)
		printf("\t- Always-on-top\n");
	if(flags & SDL_WINDOW_HIDDEN)
		printf("\t- Hidden\n");
	if(flags & SDL_WINDOW_MAXIMIZED)
		printf("\t- Maximized\n");
	if(flags & SDL_WINDOW_MINIMIZED)
		printf("\t- Minimized\n");
	if(flags & SDL_WINDOW_MOUSE_CAPTURE)
		printf("\t- Mouse-capture\n");
	if(flags & SDL_WINDOW_VULKAN)
		printf("\t- Vulkan\n");
		
	

	window = SDL_CreateWindow(settings.title, 
								SDL_WINDOWPOS_CENTERED, 
								SDL_WINDOWPOS_CENTERED, 
								settings.width, 
								settings.height, 
								flags);

	if(!window)
	{
		//@Error
		printf("Window couldn't be created!\n");
		printf("%s\n",SDL_GetError());
		return;
	}

	// Make sure window is in focus when created
	SDL_RaiseWindow(window);
	SDL_SetWindowInputFocus(window);

	setup_gl_context();

	// Update window to match settings
	
	printf("Successfully created window\n");
}

void GameWindow::swap_buffers()
{
	assert(window);
	SDL_GL_SwapWindow(window);
}

void GameWindow::setup_gl_context()
{
	assert(window);
	printf("Setting up gl context\n");
	
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24  );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3);

	context = SDL_GL_CreateContext(window);

	if(!context)
	{
		//@Error
		printf("GL Context couldn't be created!\n");
		printf("%s\n",SDL_GetError());
		return;
	}

	SDL_GL_MakeCurrent(window, context);

	printf("VSync: %s",(settings.vsync?"On":"Off"));

	glClearColor(0.3,0.6,0.8,1); // Cornflower blue <3

}

int GameWindow::get_width()
{
	return settings.width;
}

int GameWindow::get_height()
{
	return settings.height;
}

void GameWindow::set_mouse_pos(int x, int y)
{
	assert(window);
	SDL_WarpMouseInWindow(window, x, y);
}

void GameWindow::apply_settings(WindowSettings new_settings)
{
	assert(window);
	settings = new_settings;

	//
	// Find closest matching displaymode
	//

	SDL_DisplayMode target, closest;

	target.w = settings.width;
	target.h = settings.height;
	target.format = 0; // Anything
	target.refresh_rate = settings.refresh_rate;
	target.driverdata = 0; // initialize
	printf("Requesting DisplayMode: %ix%i %iHz\n",target.w, target.h, target.refresh_rate);

	if(SDL_GetClosestDisplayMode(settings.display_index, &target, &closest) == 0)
	{
		// Didn't find any matching our request, get the first one supported by the display!
		printf("Error! Requested DisplayMode not supported nor was any similar to it!\n");
		int ret = 0;
		ret = SDL_GetDesktopDisplayMode(settings.display_index, &closest);
		if(ret != 0)
		{
			printf("No supported display found! Code: %i\nSDL_Error:%s\n",ret,SDL_GetError());
			exit(-1);
		}
	}
	else
	{
		printf("Supported DisplayMode selected: %ix%i %iHz\n",closest.w,closest.h,closest.refresh_rate);

		settings.width = closest.w;
		settings.height = closest.h;
		settings.refresh_rate = closest.refresh_rate;

	}

	//
	// Assume a valid displaymode
	//

	// Get native desktop size for use in borderless fullscreen
	SDL_DisplayMode native;
	SDL_GetDesktopDisplayMode(settings.display_index, &native);


	// Apply window resize

	if(settings.fullscreen)
	{
		if(settings.borderless)
		{
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
			SDL_SetWindowSize(window, native.w, native.h);
			glViewport(0,0,native.w,native.h);
			real_width = native.w;
			real_height = native.h;
			SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED);
			if(game) game->window_resize(closest.w,closest.h);
		}
		else
		{
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
			SDL_SetWindowSize(window, closest.w, closest.h);
			glViewport(0,0,closest.w,closest.h);
			real_width = closest.w;
			real_height = closest.h;
			if(game) game->window_resize(closest.w, closest.h);
		}
	}
	else
	{
		if(settings.borderless)
		{
			SDL_SetWindowFullscreen(window, 0);
			SDL_SetWindowBordered(window, SDL_FALSE);
			SDL_SetWindowSize(window, closest.w, closest.h);
			glViewport(0,0,closest.w,closest.h);
			real_width = closest.w;
			real_height = closest.h;
			if(game) game->window_resize(closest.w, closest.h);
		}
		else
		{
			SDL_SetWindowFullscreen(window, 0);
			SDL_SetWindowBordered(window, SDL_TRUE);
			SDL_SetWindowSize(window, closest.w, closest.h);
			glViewport(0,0,closest.w,closest.h);
			real_width = closest.w;
			real_height = closest.h;
			if(game) game->window_resize(closest.w,closest.h);
		}
	}


	// -1 possible for late-swap tearing, not used here though
	if(settings.vsync)
		SDL_GL_SetSwapInterval(1); 
	else
		SDL_GL_SetSwapInterval(0);

}


WindowSettings GameWindow::get_settings()
{
	return settings;
}

int GameWindow::get_real_screen_width()
{
	return real_width;
}

int GameWindow::get_real_screen_height()
{
	return real_height;
}