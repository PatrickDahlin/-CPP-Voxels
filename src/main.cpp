#define GL3_PROTOTYPES 1
#include <GL\glew.h>
#include <SDL2\SDL.h>
#include <iostream>
#include "headers/core/GameWindow.hpp"
#include "headers/core/Errors.hpp"
#include "headers/core/ShaderProgram.hpp"

int main(int argc, char* argv[])
{
	SDL_ClearError();
	if(SDL_Init( SDL_INIT_VIDEO ) < 0)
	{
		error("SDL failed to initialize");
		return -1;
	}

	GameWindow game_window = GameWindow("Game", 1280, 720, SDL_WINDOW_OPENGL);


	glewInit();

	GLuint vao_id;
	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);

	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	const char* vert = "#version 330 core \n\
	in vec3 pos;\
	void main(){\
	gl_Position = vec4(pos,1);\
	}";
	const char* frag = "#version 330 core \n\
	out vec4 frag_color;\
	void main(){\
	frag_color = vec4(1,0,0,1);\
	}";

	ShaderProgram shader = ShaderProgram(vert, frag);
//*/
	bool run = true;
	while(run)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			if(event.key.keysym.sym == SDLK_ESCAPE)
				run = false;
		}
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		shader.use();

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(
			0,			// Attrib 0, must match layout in shader
			3,			// size
			GL_FLOAT,	// type
			GL_FALSE,	// normalized?
			0,			// stride
			(void*)0	// offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glDisableVertexAttribArray(0);
//*/
		game_window.swap_buffers();
		SDL_Delay(16); // 16 ms is about 60 fps
	}

	//glUseProgram(0);
	SDL_Quit();
	return 0;
}