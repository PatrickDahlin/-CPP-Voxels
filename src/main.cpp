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

	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.5f, 0.0f
	};


	GLBuffer myBuffer((void*)g_vertex_buffer_data, sizeof(g_vertex_buffer_data), 3);


	//
	// Shader stuff
	//

	std::string vert = read_file("src/shaders/Basic-vert.glsl");
	std::string frag = read_file("src/shaders/Basic-frag.glsl");
	ShaderProgram shader = ShaderProgram(vert.data(), frag.data());

	Transform myTransform;

	myTransform.translate(1,0,0);
	myTransform.rotate(90,0,0);
	
	glm::mat4 m = myTransform.get_combined();
	printf("%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n%f,%f,%f,%f\n",m[0][0],m[1][0],m[2][0],m[3][0],
																	m[0][1],m[1][1],m[2][1],m[3][1],
																	m[0][2],m[1][2],m[2][2],m[3][2],
																	m[0][3],m[1][3],m[2][3],m[3][3]);
//*/
	Input myInput;

	//
	// Main loop
	//

	bool run = true;
	while(run)
	{
		myInput.poll_events();

		if(myInput.get_key(SDLK_ESCAPE) == KeyState::PRESSED)
			exit(0);


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();

		myBuffer.bind();
		myBuffer.data_pointer(0, 3, GL_FLOAT, 3 * sizeof(float), true, BUFFER_OFFSET(0));
		myBuffer.draw();
		myBuffer.unbind();

		game_window.swap_buffers();
		SDL_Delay(16); // 16 ms is about 60 fps
	}
	

	// Unbind vao
	glBindVertexArray(0);

	game_window.destroy();

	SDL_Quit();
	return 0;
}