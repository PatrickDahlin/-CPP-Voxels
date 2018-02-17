#include "core/GameWindow.hpp"
#include "core/Errors.hpp"

GameWindow::GameWindow(const char* title, int width, int height, SDL_WindowFlags flags) : 
window(NULL), context(NULL), flags(flags), title(title), width(width), height(height), vsync(false)
{
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

void GameWindow::set_window_flags(SDL_WindowFlags flags)
{
	this->flags = flags;
}

void GameWindow::create_window()
{
	SDL_ClearError();

	std::cout << "Setting up window with settings:" << std::endl;
	std::cout << "\t" << width << "x" << height << std::endl;
	if(flags & SDL_WINDOW_FULLSCREEN)
		std::cout << "\t- Fullscreen" << std::endl;
	if(flags & SDL_WINDOW_BORDERLESS)
		std::cout << "\t- Borderless" << std::endl;
	if(flags & SDL_WINDOW_OPENGL)
		std::cout << "\t- OpenGL" << std::endl;
	if(flags & SDL_WINDOW_FULLSCREEN_DESKTOP)
		std::cout << "\t- Fullscreen-Desktop" << std::endl;
	if(flags & SDL_WINDOW_INPUT_GRABBED)
		std::cout << "\t- Input-grabbed" << std::endl;
	if(flags & SDL_WINDOW_ALLOW_HIGHDPI)
		std::cout << "\t- Allow-High-DPI" << std::endl;
	if(flags & SDL_WINDOW_ALWAYS_ON_TOP)
		std::cout << "\t- Always-on-top" << std::endl;
	if(flags & SDL_WINDOW_HIDDEN)
		std::cout << "\t- Hidden" << std::endl;
	if(flags & SDL_WINDOW_MAXIMIZED)
		std::cout << "\t- Maximized" << std::endl;
	if(flags & SDL_WINDOW_MINIMIZED)
		std::cout << "\t- Minimized" << std::endl;
	if(flags & SDL_WINDOW_MOUSE_CAPTURE)
		std::cout << "\t- Mouse-capture" << std::endl;
	if(flags & SDL_WINDOW_VULKAN)
		std::cout << "\t- Vulkan" << std::endl;
		
	
	std::cout << std::endl;

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);

	if(!window)
	{
		error("Window couldn't be created!");
		error(SDL_GetError());
		return;
	}

	// Make sure window is in focus when created
	SDL_RaiseWindow(window);
	SDL_SetWindowInputFocus(window);

	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24  );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3);

	context = SDL_GL_CreateContext(window);

	if(!context)
	{
		error("GL Context couldn't be created!");
		error(SDL_GetError());
		return;
	}

	SDL_GL_MakeCurrent(window, context);

	setup_gl_context();

	std::cout << "Successfully created window" << std::endl;
}

void GameWindow::swap_buffers()
{
	if(window)
		SDL_GL_SwapWindow(window);
}

void GameWindow::setup_gl_context()
{
	std::cout << "Setting up gl context" << std::endl;
	
	// -1 possible for late-swap tearing, not used here though
	set_vsync(vsync);

	std::cout << "VSync: ";
	if(vsync)
		std::cout << "On" << std::endl;
	else
		std::cout << "Off" << std::endl;

	glClearColor(0.3,0.6,0.8,1);
	glViewport(0,0, width, height);
}

void GameWindow::set_vsync(bool vsync)
{
	if(vsync)
		SDL_GL_SetSwapInterval(1); 
	else
		SDL_GL_SetSwapInterval(0);
	
	this->vsync = vsync;
}

int GameWindow::get_width()
{
	return width;
}

int GameWindow::get_height()
{
	return height;
}


void GameWindow::set_mouse_pos(int x, int y)
{
	SDL_WarpMouseInWindow(window, x, y);
}