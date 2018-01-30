#define GL3_PROTOTYPES 1
#include <GL\glew.h>
#include <SDL2\SDL.h>
#include <iostream>
#include <cstdio>
#include <fstream>
#include "headers/core/GameWindow.hpp"
#include "headers/core/ShaderProgram.hpp"
#include "headers/core/Files.hpp"
#include "headers/core/Macros.hpp"
#include "headers/core/GLBuffer.hpp"
#include "headers/core/Input.hpp"
#include "headers/core/Transform.hpp"
#include "headers/core/Material.hpp"
#include "headers/core/RenderPass.hpp"
#include "headers/core/VertexArray.hpp"
#include "headers/core/Camera.hpp"


/*


TODO:

	RenderPass shouldn't use GLBuffer to draw! Perhaps use a Mesh wrapper for buffer or a VertexArray rather
	Materials aren't usable yet, need a way to upload the data, !Uniform buffer objects!
		Same thing for uploading/binding camera matrices to shaders

	Camera shouldn't use Transform to get viewmatrix
		Use position and lookat vector instead and get view matrix from glm

*/




int main(int argc, char* argv[])
{
	SDL_ClearError();
	if(SDL_Init( SDL_INIT_VIDEO ) < 0)
	{
		error("SDL failed to initialize");
		return -1;
	}

	GameWindow game_window = GameWindow("Game", 1280, 720, SDL_WINDOW_OPENGL);


	GLenum res = glewInit();
	if(res != GLEW_OK)
	{
		error("GLEW failed to initialize!");
		return -1;
	}




	// GLBuffer needs a Vao bound at all times
	// this is new in GL 3.3+ ..why?.. Q.Q
	GLuint vao_id;
	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);


	//
	// Shader stuff
	//

	std::string vert = read_file("src/shaders/Basic-vert.glsl");
	std::string frag = read_file("src/shaders/Basic-frag.glsl");
	ShaderProgram shader = ShaderProgram(vert.data(), frag.data());

	CHECK_GL_ERROR();

	Material mat;
	mat.tint = Color(127,127,127,255);

	Camera cam(90.0f, 1280, 720, 0.1f, 100.0f);

	Input myInput;

	//
	// Main loop
	//


	VertexArray myVertArr;

	myVertArr.add_vertex(-100,-100,2);
	myVertArr.add_color(1,1,0,1);
	myVertArr.add_normal(1,0,0);

	myVertArr.add_vertex(-100,100,-2);
	myVertArr.add_color(0,0,1,1);
	myVertArr.add_normal(1,0,0);

	myVertArr.add_vertex(100,100,2);
	myVertArr.add_color(0,1,0,1);
	myVertArr.add_normal(1,0,0);

	myVertArr.upload_data();

	CHECK_GL_ERROR();

	bool run = true;
	while(run)
	{
		myInput.poll_events();

		if(myInput.get_key(SDLK_ESCAPE) == KeyState::PRESSED)
			exit(0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		


		RenderPass myPass;
		
		myPass.draw_model(&myVertArr, &shader, &mat, &cam);

		myPass.do_render();



		game_window.swap_buffers();
		SDL_Delay(16); // 16 ms is about 60 fps
	}
	
	myVertArr.clear();

	// Unbind vao
	glBindVertexArray(0);

	game_window.destroy();

	SDL_Quit();
	return 0;
}