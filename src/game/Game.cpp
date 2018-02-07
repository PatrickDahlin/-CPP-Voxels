#include "game/Game.hpp"
#include "core/Macros.hpp"
#include "graphics/RenderPass.hpp"
#include "graphics/GLTexture.hpp"
#include "graphics/ShaderProgram.hpp"
#include "graphics/OrthographicCamera.hpp"
#include "graphics/VertexArray.hpp"
#include "graphics/GLBuffer.hpp"
#include "core/Time.hpp"
#include "core/Files.hpp"
#include "graphics/ImGuiRenderer.hpp"

#include <imgui/imgui.h>

bool 			Game::running = true;

Game::Game(GameWindow* window) :
game_window(window),
input(window),
main_scene(nullptr)
{
	printf("Setting up Game\n");
	uicam = new OrthographicCamera(0, window->get_width(), 0, window->get_height());

	input.show_cursor(false);
	input.set_lock_mouse(false);
	
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(window->get_width(), window->get_height());
	io.RenderDrawListsFn = render_imgui; 


	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	// TODO: At this points you've got the texture data and you need to upload that your your graphic system:
	GLTexture* myTex = new GLTexture(ColorFormat::RGBA, pixels, width, height);
	// TODO: Store your texture pointer/identifier (whatever your engine uses) in 'io.Fonts->TexID'. This will be passed back to your via the renderer.
	io.Fonts->TexID = (void*)myTex;

	std::string vert = read_file("data/shaders/ImGui-vert.glsl");
	std::string frag = read_file("data/shaders/ImGui-frag.glsl");
	std::string header = read_file("data/shaders/Shader_Header.glsl");
	imgui_shader = new ShaderProgram(vert.c_str(), frag.c_str(), header.c_str());

	//glScissor(0,0,window->get_width(), window->get_height());
}

Game::~Game()
{}

void Game::load()
{
	main_scene = new MainScene(&input, &scene_manager);
	scene_manager.switch_to_scene(main_scene);

	glClearColor(0.3f, 0.5f, 0.8f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_SCISSOR_TEST);
	glViewport(0,0,game_window->get_width(),game_window->get_height());
}

void Game::run()
{
	auto last_frame = Clock::now();
	float delta_time = 0.0f;

	Time::set_start(); // Only accessible here, sets starting point of application and
						// is used in Time::time_since_startup_sec();

	while(running)
	{
		last_frame = Clock::now();

		input.poll_events();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = delta_time;
		io.MousePos = ImVec2(input.get_mouse_pos().x, input.get_mouse_pos().y);
		io.MouseDown[0] = input.get_mouse_btn(0) == KeyState::PRESSED ||
							input.get_mouse_btn(0) == KeyState::REPEAT;
		io.MouseDown[1] = input.get_mouse_btn(1) == KeyState::PRESSED ||
							input.get_mouse_btn(1) == KeyState::REPEAT;
		
		ImGui::NewFrame();

		
		// @Temporary
		if(input.get_key(SDLK_ESCAPE) == KeyState::PRESSED)
			Game::quit();

		if(input.get_key(SDLK_l) == KeyState::PRESSED)
		{
			static bool locked = true;
			locked = !locked;
			input.set_lock_mouse(locked);
			input.show_cursor(!locked);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderPass* pass = new RenderPass;

		scene_manager.update(delta_time);
		scene_manager.render(pass);
		
		pass->do_render();

		
		imgui_shader->use();

		const glm::mat4 ortho_proj =
		glm::mat4(
			glm::vec4( 2.0f/io.DisplaySize.x, 0.0f,                   0.0f, 0.0f ),
			glm::vec4( 0.0f,                  2.0f/-io.DisplaySize.y, 0.0f, 0.0f ),
			glm::vec4( 0.0f,                  0.0f,                  -1.0f, 0.0f ),
			glm::vec4(-1.0f,                  1.0f,                   0.0f, 1.0f )
		);

		imgui_shader->upload_projection(ortho_proj);//uicam->get_projection());
		imgui_shader->upload_view(uicam->get_view());
		imgui_shader->upload_model(glm::mat4(1.0f));
		ImGui::Render(); 

		game_window->swap_buffers();
		delete pass;

		//
		// Frametime calculation
		//
		// Calculate how long processing took, we need to calculate how long we need to wait for fps limit
		auto delta = Clock::now() - last_frame;
		double delta_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(delta).count() / 1000000.0;
		
		if(delta_ms < 16)
		{
			SDL_Delay((int)(16 - delta_ms));
		}

		// Calculate the deltatime for this frame including the fps-limit time
		delta = Clock::now() - last_frame;
		delta_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(delta).count() / 1000000.0;
		delta_time = (float)(delta_ms / 1000.0);
	}

	scene_manager.dispose();
}

void Game::quit()
{
	running = false;
	coutln("Reqested quit");
}

