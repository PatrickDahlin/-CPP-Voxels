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


/*


TODO:

	RenderPass shouldn't use GLBuffer to draw! Perhaps use a Mesh wrapper for buffer or a VertexArray rather
	Materials aren't usable yet, need a way to upload the data, !Uniform buffer objects!
		Same thing for uploading/binding camera matrices to shaders


vertexarray.addVertex(vec3)
vertexarray.addNorm(vec3)
vertexarray.addColor(vec3)
vertexarray.addUv(vec2)

vertexarray.addTangent(vec3)
vertexarray.addBitangent(vec3)


class VertexArray
{
	vector<GLBuffer> buffers
	vector<DataPointer> ptrs
}

class Mesh
{
	VertexArray	arr

}



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

	Material mat;
	mat.tint = Color(127,127,127,255);

	Input myInput;

	//
	// Main loop
	//

	myBuffer.bind();
	myBuffer.data_pointer(0, 3, GL_FLOAT, 3 * sizeof(float), true, BUFFER_OFFSET(0));
		

	bool run = true;
	while(run)
	{
		myInput.poll_events();

		if(myInput.get_key(SDLK_ESCAPE) == KeyState::PRESSED)
			exit(0);


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//RenderPass myPass;
		
		//myPass.draw_model(&myBuffer, &shader, &mat);

		//myPass.do_render();

		shader.use();

		myBuffer.draw();
		//myBuffer.unbind();

		game_window.swap_buffers();
		SDL_Delay(16); // 16 ms is about 60 fps
	}
	

	// Unbind vao
	glBindVertexArray(0);

	game_window.destroy();

	SDL_Quit();
	return 0;
}