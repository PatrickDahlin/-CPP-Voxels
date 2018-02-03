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
		- (H) Actor class []
		- (H) Text rendering []
		- (M) Uniform buffer objects []
			- (M) Material struct uploading to shader []
		- (M) Debug class for collecting debug info []
		- (M) Nanovg ui package perhaps []
		- (M) OpenAL for sound []
		- (H) Model class implementation [x]
		- (H) Scene class [x]
		- (H) Cleanup main [x]
		- (H) Scene management [x]


	Lower prio TODO
		- (L) GameWindow really needs an openwindow function instead of
			opening window in constructor []


	main should only
		- Create window
		- Set up game instance and run it
		- Clean up window
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





// @Cleanup anything after this, it's left for reference only atm
// Old main
	/*
	//
	// Shader stuff
	//

	std::string vert = read_file("data/shaders/Basic-vert.glsl");
	std::string frag = read_file("data/shaders/Basic-frag.glsl");
	std::string header = read_file("data/shaders/Shader_Header.glsl");
	ShaderProgram shader = ShaderProgram(vert.data(), frag.data(), header.data());

	CHECK_GL_ERROR();

	Material mat;
	mat.tint = Color(127,127,127,255);

	Camera cam(3.14f/4.0f, 1280, 720, 0.1f, 5000.0f);

	Input myInput;
	myInput.set_lock_mouse(true);
	myInput.show_cursor(false);

	//
	// Main loop
	//

	//make_cube(myVertArr, glm::vec4(0,1,0,1), glm::vec3(0,0,0), 40);

	//myVertArr.upload_data();

	
	int width, height, channelnr;
	width = 0;
	height = 0;
	unsigned char* data = stbi_load("data/textures/grass.jpg", &width, &height, &channelnr, 0);
	
	GLTexture* myTexture = new GLTexture(data, width, height);
	
	mat.texture = myTexture;

	Model model;
	model.set_material(&mat);

	make_cube(model, glm::vec4(0,1,0,1), glm::vec3(0,0,0), 40);

	// Free the data when no longer needed
	stbi_image_free(data);

	glEnable(GL_DEPTH_TEST);

	CHECK_GL_ERROR();

	bool run = true;
	while(run)
	{
		myInput.poll_events();

		if(myInput.get_key(SDLK_ESCAPE) == KeyState::PRESSED)
			exit(0);
		if(myInput.get_key(SDLK_w) == KeyState::REPEAT || 
			myInput.get_key(SDLK_w) == KeyState::PRESSED)
			cam.translate( cam.get_forward() * -10.0f);
	
		if(myInput.get_key(SDLK_s) == KeyState::REPEAT || 
			myInput.get_key(SDLK_s) == KeyState::PRESSED)
			cam.translate( cam.get_forward() * 10.0f);

		if(myInput.get_key(SDLK_a) == KeyState::REPEAT || 
			myInput.get_key(SDLK_a) == KeyState::PRESSED)
			cam.translate( cam.get_right() * -10.0f);
	
		if(myInput.get_key(SDLK_d) == KeyState::REPEAT || 
			myInput.get_key(SDLK_d) == KeyState::PRESSED)
			cam.translate( cam.get_right() * 10.0f);
		
		if(myInput.get_key(SDLK_LEFT) == KeyState::REPEAT || 
			myInput.get_key(SDLK_LEFT) == KeyState::PRESSED)
			cam.rotate( 0, 0.03f, 0 );
		
		if(myInput.get_key(SDLK_RIGHT) == KeyState::REPEAT || 
			myInput.get_key(SDLK_RIGHT) == KeyState::PRESSED)
			cam.rotate( 0, -0.03f, 0 );

		if(myInput.get_key(SDLK_UP) == KeyState::REPEAT || 
			myInput.get_key(SDLK_UP) == KeyState::PRESSED)
			cam.rotate( 0.03f, 0, 0 );

		if(myInput.get_key(SDLK_DOWN) == KeyState::REPEAT || 
			myInput.get_key(SDLK_DOWN) == KeyState::PRESSED)
			cam.rotate( -0.03f, 0, 0 );

		if(myInput.get_key(SDLK_e) == KeyState::REPEAT || 
			myInput.get_key(SDLK_e) == KeyState::PRESSED)
			cam.translate( cam.get_up() * 10.0f);

		if(myInput.get_key(SDLK_q) == KeyState::REPEAT || 
			myInput.get_key(SDLK_q) == KeyState::PRESSED)
			cam.translate( cam.get_up() * -10.0f);
		
		cam.rotate( -myInput.get_mouse_pos_delta().y * 0.01f ,
					-myInput.get_mouse_pos_delta().x * 0.01f, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		RenderPass myPass;
		
		myPass.draw_model(&model, &shader, &cam);

		myPass.do_render();



		game_window.swap_buffers();
		SDL_Delay(16); // 16 ms is about 60 fps
	}
	
	model.dispose(); // @cleanup
	*/