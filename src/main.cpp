#define GL3_PROTOTYPES 1
#include <GL\glew.h>
#include <SDL2\SDL.h>
#include <iostream>
#include <cstdio>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "core/GameWindow.hpp"
#include "graphics/ShaderProgram.hpp"
#include "core/Files.hpp"
#include "core/Macros.hpp"
#include "graphics/GLBuffer.hpp"
#include "core/Input.hpp"
#include "core/Transform.hpp"
#include "graphics/Material.hpp"
#include "graphics/RenderPass.hpp"
#include "graphics/VertexArray.hpp"
#include "graphics/Model.hpp"
#include "graphics/Camera.hpp"
#include "graphics/GLTexture.hpp"



/*


TODO:

	Materials aren't usable yet, need a way to upload the data, !Uniform buffer objects!
		Same thing for uploading/binding camera matrices to shaders



	Heaps for prioritized jobs

	TODO:
		- Cleanup main []
		- Scene management []
		- Scene class []
		- Actor class []
		- Model class implementation [x]

*/


void make_cube(Model& model, glm::vec4 col, glm::vec3 pos, float scale)
{
	float n = -0.5f;
	float p = 0.5f;

	std::vector<glm::vec3> verts;
	std::vector<glm::vec4> colors;
	std::vector<glm::vec2> texcoords;


	// FRONT
	verts.emplace_back(pos + glm::vec3(n,n,n) * scale);
	verts.emplace_back(pos + glm::vec3(p,n,n) * scale);
	verts.emplace_back(pos + glm::vec3(p,p,n) * scale);	
	verts.emplace_back(pos + glm::vec3(n,n,n) * scale);
	verts.emplace_back(pos + glm::vec3(p,p,n) * scale);
	verts.emplace_back(pos + glm::vec3(n,p,n) * scale);

	colors.emplace_back(col); colors.emplace_back(col); colors.emplace_back(col);
	colors.emplace_back(col); colors.emplace_back(col); colors.emplace_back(col);

	texcoords.emplace_back(0,0); texcoords.emplace_back(1,0); texcoords.emplace_back(1,1);
	texcoords.emplace_back(0,0); texcoords.emplace_back(1,1); texcoords.emplace_back(0,1);

	// TOP
	verts.emplace_back(pos + glm::vec3(n,p,n) * scale);
	verts.emplace_back(pos + glm::vec3(p,p,p) * scale);
	verts.emplace_back(pos + glm::vec3(p,p,n) * scale);
	verts.emplace_back(pos + glm::vec3(n,p,n) * scale);
	verts.emplace_back(pos + glm::vec3(p,p,p) * scale);
	verts.emplace_back(pos + glm::vec3(n,p,p) * scale);

	colors.emplace_back(col); colors.emplace_back(col); colors.emplace_back(col);
	colors.emplace_back(col); colors.emplace_back(col); colors.emplace_back(col);
	
	texcoords.emplace_back(0,0); texcoords.emplace_back(1,0); texcoords.emplace_back(1,1);
	texcoords.emplace_back(0,0); texcoords.emplace_back(1,1); texcoords.emplace_back(0,1);

	// LEFT
	verts.emplace_back(pos + glm::vec3(n,n,p) * scale);
	verts.emplace_back(pos + glm::vec3(n,p,n) * scale);
	verts.emplace_back(pos + glm::vec3(n,n,n) * scale);
	verts.emplace_back(pos + glm::vec3(n,n,p) * scale);
	verts.emplace_back(pos + glm::vec3(n,p,n) * scale);
	verts.emplace_back(pos + glm::vec3(n,p,p) * scale);

	colors.emplace_back(col); colors.emplace_back(col); colors.emplace_back(col);
	colors.emplace_back(col); colors.emplace_back(col); colors.emplace_back(col);

	texcoords.emplace_back(0,0); texcoords.emplace_back(1,0); texcoords.emplace_back(1,1);
	texcoords.emplace_back(0,0); texcoords.emplace_back(1,1); texcoords.emplace_back(0,1);

	// RIGHT
	verts.emplace_back(pos + glm::vec3(p,n,n) * scale);
	verts.emplace_back(pos + glm::vec3(p,n,p) * scale);
	verts.emplace_back(pos + glm::vec3(p,p,p) * scale);
	verts.emplace_back(pos + glm::vec3(p,n,n) * scale);
	verts.emplace_back(pos + glm::vec3(p,p,p) * scale);
	verts.emplace_back(pos + glm::vec3(p,p,n) * scale);
	
	colors.emplace_back(col); colors.emplace_back(col); colors.emplace_back(col);
	colors.emplace_back(col); colors.emplace_back(col); colors.emplace_back(col);
	
	texcoords.emplace_back(0,0); texcoords.emplace_back(1,0); texcoords.emplace_back(1,1);
	texcoords.emplace_back(0,0); texcoords.emplace_back(1,1); texcoords.emplace_back(0,1);

	// BOTTOM
	verts.emplace_back(pos + glm::vec3(n,n,p) * scale);
	verts.emplace_back(pos + glm::vec3(p,n,p) * scale);
	verts.emplace_back(pos + glm::vec3(p,n,n) * scale);
	verts.emplace_back(pos + glm::vec3(n,n,p) * scale);
	verts.emplace_back(pos + glm::vec3(p,n,n) * scale);
	verts.emplace_back(pos + glm::vec3(n,n,n) * scale);
	
	colors.emplace_back(col); colors.emplace_back(col); colors.emplace_back(col);
	colors.emplace_back(col); colors.emplace_back(col); colors.emplace_back(col);
	
	texcoords.emplace_back(0,0); texcoords.emplace_back(1,0); texcoords.emplace_back(1,1);
	texcoords.emplace_back(0,0); texcoords.emplace_back(1,1); texcoords.emplace_back(0,1);

	// BACK
	verts.emplace_back(pos + glm::vec3(n,n,p) * scale);
	verts.emplace_back(pos + glm::vec3(p,p,p) * scale);
	verts.emplace_back(pos + glm::vec3(p,n,p) * scale);
	verts.emplace_back(pos + glm::vec3(n,n,p) * scale);
	verts.emplace_back(pos + glm::vec3(n,p,p) * scale);
	verts.emplace_back(pos + glm::vec3(p,p,p) * scale);
	
	colors.emplace_back(col); colors.emplace_back(col); colors.emplace_back(col);
	colors.emplace_back(col); colors.emplace_back(col); colors.emplace_back(col);
	
	texcoords.emplace_back(0,0); texcoords.emplace_back(1,0); texcoords.emplace_back(1,1);
	texcoords.emplace_back(0,0); texcoords.emplace_back(1,1); texcoords.emplace_back(0,1);

	model.set_vertices(verts);
	model.set_colors(colors);
	model.set_texcoords(texcoords);
}


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




	// GLBuffer needs a Vao bound at all times if you draw it
	// Should not be used tho.......
	// this is new in GL 3.3+ ..why?.. Q.Q
	GLuint vao_id;
	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);


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
	
	model.dispose();

	// Unbind vao
	glBindVertexArray(0);

	game_window.destroy();

	SDL_Quit();
	return 0;
}