#include "core/GameWindow.hpp"
#include <iostream>
#include <stdio.h>

GameWindow::GameWindow(WindowSettings settings) ://(const char* title, int width, int height, int flags) : 
window(NULL), context(NULL)
{
	this->settings = settings;
	create_window();
}

GameWindow::~GameWindow()
{
	destroy();
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

void GameWindow::set_fullscreen(bool fullscreen)
{
	assert(window);
	// SDL_WINDOW_FULLSCREEN is true fullscreen
	// while SDL_WINDOW_FULLSCREEN_DESKTOP is windowed with size of desktop
	if(fullscreen && !settings.fullscreen)
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	else if(!fullscreen && settings.fullscreen)
		SDL_SetWindowFullscreen(window, 0);

	settings.fullscreen = fullscreen;
}

void GameWindow::set_window_size(int w, int h)
{
	assert(window);
	settings.width = w;
	settings.height = h;
	SDL_SetWindowSize(window, w, h);
}

void GameWindow::create_window()
{
	SDL_ClearError();

	int flags = ( settings.is_vulkan ? SDL_WINDOW_VULKAN : SDL_WINDOW_OPENGL );

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
		
	
	std::cout << std::endl;

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

	// -1 possible for late-swap tearing, not used here though
	set_vsync(settings.vsync);

	printf("VSync: %s",(settings.vsync?"On":"Off"));

	glClearColor(0.3,0.6,0.8,1);
	glViewport(0,0, settings.width, settings.height);
}

void GameWindow::set_vsync(bool vsync)
{
	if(vsync)
		SDL_GL_SetSwapInterval(1); 
	else
		SDL_GL_SetSwapInterval(0);
	
	settings.vsync = vsync;
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

WindowSettings GameWindow::get_settings()
{
	return settings;
}