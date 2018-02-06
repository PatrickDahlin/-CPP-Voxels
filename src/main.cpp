#define GL3_PROTOTYPES 1
#include <GL\glew.h>
#include <SDL2\SDL.h>
#include <iostream>

#include "game/Game.hpp"
#include "core/Macros.hpp"
#include "core/GameWindow.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

/*


TODO:

	Materials aren't usable yet, need a way to upload the data, !Uniform buffer objects!
		Same thing for uploading/binding camera matrices to shaders

	"packages"
		core
		graphics
		physics @future

	Heaps for prioritized jobs

	TODO:
		- (M) Actor class []
		- (M) Text rendering []
		- (M) Uniform buffer objects []
			- (M) Material struct uploading to shader []
		- (M) Debug class for collecting debug info []
		- (H) Camera class needs perspective/ortho switch []
			- Camera abstract class with Orthographic and Perspective camera subclass []
		- (H) Element buffer for vertexarray [x] @Untested @TODO
		- (H) Multi-camera rendering (Renderpass->RenderBatch) []
		- (M) OpenAL for sound []


	Done:
		- (H) ImGui for debug ui rendering
		- (H) Texture color format option [x]
		- (H) Model class implementation [x]
		- (H) Scene class [x]
		- (H) Cleanup main [x]
		- (H) Scene management [x]


	Lower prio TODO
		- (L) GameWindow really needs an openwindow function instead of
			opening window in constructor []


	Licensing : stb source has 2 licenses to choose from

*/


int main(int argc, char* argv[])
{
	//
	// SDL
	//
	SDL_ClearError();
	if(SDL_Init( SDL_INIT_EVERYTHING ) < 0)
	{
		error("SDL failed to initialize");
		// @TODO Get sdl error and log it
		return -1;
	}

	GameWindow* window = new GameWindow("Game", 1280, 720, SDL_WINDOW_OPENGL);

	//
	// GLEW
	//
	GLenum res = glewInit();
	if(res != GLEW_OK)
	{
		error("GLEW failed to initialize!");
		coutln(glewGetErrorString(res));
		return -1;
	}
	
	Game game(window);

	game.load();
	game.run();
	
	window->destroy();
	delete window;

	coutln("Closed game, cleaning up window");
	
	SDL_Quit();
	coutln("Bai");
	return 0;
}
