#define GL3_PROTOTYPES 1
#include <GL\glew.h>
#include <SDL2\SDL.h>
#include <iostream>
#include <fstream>
#include "headers/core/GameWindow.hpp"
#include "headers/core/ShaderProgram.hpp"
#include "headers/core/Files.hpp"
#include "headers/core/Macros.hpp"
#include "headers/core/GLBuffer.hpp"
#include "headers/core/Thread.hpp"


/*

SDL_Event event;
    while (F_EVENT_LOOP == RUNNING)
    {
        if (SDL_WaitEvent(&event)) // execution suspends here while waiting on an event
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    printf("SDL_QUIT signal received.\n");
                    F_EVENT_LOOP = STOPPED;
            }
        }
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


	//
	// Main loop
	//

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

		myBuffer.bind();
		myBuffer.data_pointer(0, 3, GL_FLOAT, 3 * sizeof(float), true, BUFFER_OFFSET(0));
		myBuffer.draw();
		myBuffer.unbind();

		game_window.swap_buffers();
		SDL_Delay(100); // 16 ms is about 60 fps
	}
	

	// Unbind vao
	glBindVertexArray(0);

	game_window.destroy();

	SDL_Quit();
	return 0;
}