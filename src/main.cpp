#define GL3_PROTOTYPES 1
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <iostream>

#include "game/Game.hpp"
#include "core/Common.hpp"
#include "core/GameWindow.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* file
#include <tiny_obj_loader.h>

/*


TODO:

	Materials aren't usable yet, need a way to upload the data, !Uniform buffer objects!

	"packages"
		core
		graphics
		physics @future
		voxels

	Heaps for prioritized jobs

	ResourceManager handling
		[Done] Texture-loading
		[Done] Shader-loading
		3d-model-loading
		soundfile-loading

	Bugs:
		- [Fixed] ShaderManager and TextureManager have some resource problems, segfaults are happening...
		- [Fixed] DebugCamera freaks out at low framerates

	Woking on:
		- (H) Frustum culling

	TODO:
		- (H) Terrain LOD system
		- (L) Load/Save window settings
		- (L) Octree data-structure for later voxel use (Later optimization)
		- (M) Actor class []
		- (M) Text rendering []
		- (M) Uniform buffer objects []
		- (M) Material struct uploading to shader []
		- (L) Debug class for collecting debug info []
		- (H) Multi-camera rendering (Renderpass->RenderBatch) []
		- (L) OpenAL for sound []
		- (L) Smoothed Input axes (Think mouse/controller axis in Unity)

	Done:
		- (H) AABB for models
		- GameWindow needs a GetSupportedDisplayModes to get a list of WindowSettings for use in options dropdown
		- Fullscreen borderless forces native screen size no more, FBOs might fixed this (change viewport before rendering to target framebuffer)
		- (L) GameWindow DisplayMode support for multiple refreshrate options [x]
		- Marching cubes implementation
		- Perlin noise implementation
		- Wireframe mode for rendering
		- (H) Element buffer for vertexarray [x] @Untested @TODO
		- (M) Camera abstract class with Orthographic and Perspective camera subclass [x]
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
		//@Error
		printf("SDL failed to initialize\n");
		// @TODO Get sdl error and log it
		return -1;
	}

	
	WindowSettings settings {
		"Game", 1280, 720,
		60, 0,
		false, false,
		false, false
	};

	GameWindow* window = new GameWindow(settings);

	//
	// GLEW
	//
	GLenum res = glewInit();
	if(res != GLEW_OK)
	{
		//@Error
		printf("GLEW failed to initialize!\n");
		printf("%s\n",glewGetErrorString(res));
		return -1;
	}

	// Flip images right side up since opengl has y axis flipped
	stbi_set_flip_vertically_on_load(true);
	
	Game* game = new Game(window);
	window->attach_game(game); // Give window the game handle to send resize "event"

	game->load();
	game->run();
	
	delete game;

	window->destroy();
	delete window;

	printf("Closed game, cleaning up window\n");
	
	SDL_Quit();
	printf("Bai\n");
	return 0;
}
