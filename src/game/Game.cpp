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
#include <iostream>

bool 			Game::running = true;

static const char* ImGui_GetClipboardText(void*)
{
    return SDL_GetClipboardText();
}

static void ImGui_SetClipboardText(void*, const char* text)
{
    SDL_SetClipboardText(text);
}

Game::Game(GameWindow* window) :
game_window(window),
input(window),
main_scene(nullptr)
{
	printf("Setting up Game\n");
	uicam = new OrthographicCamera(0, window->get_width(), 0, window->get_height());

	input.show_cursor(false);
	input.set_lock_mouse(true);
	
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(window->get_width(), window->get_height());
	io.RenderDrawListsFn = render_imgui; 
	io.SetClipboardTextFn = ImGui_SetClipboardText;
	io.GetClipboardTextFn = ImGui_GetClipboardText;
	io.KeyMap[ImGuiKey_Tab] = SDL_SCANCODE_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
    io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
    io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
    io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
    io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
    io.KeyMap[ImGuiKey_Insert] = SDL_SCANCODE_INSERT;
    io.KeyMap[ImGuiKey_Delete] = SDL_SCANCODE_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
    io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
    io.KeyMap[ImGuiKey_Escape] = SDL_SCANCODE_ESCAPE;
    io.KeyMap[ImGuiKey_A] = SDL_SCANCODE_A;
    io.KeyMap[ImGuiKey_C] = SDL_SCANCODE_C;
    io.KeyMap[ImGuiKey_V] = SDL_SCANCODE_V;
    io.KeyMap[ImGuiKey_X] = SDL_SCANCODE_X;
    io.KeyMap[ImGuiKey_Y] = SDL_SCANCODE_Y;
	io.KeyMap[ImGuiKey_Z] = SDL_SCANCODE_Z;

	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
	GLTexture* myTex = new GLTexture(ColorFormat::RGBA, pixels, width, height);
	io.Fonts->TexID = (void*)myTex;
	// TODO clean up this texture

	std::string vert = read_file("data/shaders/ImGui-vert.glsl");
	std::string frag = read_file("data/shaders/ImGui-frag.glsl");
	std::string header = read_file("data/shaders/Shader_Header.glsl");
	imgui_shader = new ShaderProgram(vert.c_str(), frag.c_str(), header.c_str());


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
		
		
		static bool imgui_captured = false;
		if(io.WantCaptureKeyboard || io.WantCaptureMouse)
		{
			if(!imgui_captured)
			{
				imgui_captured = true;
				input.set_input_enabled(false);
			}
		}
		else if(!input.is_enabled() && imgui_captured)
		{
			input.set_input_enabled(true);
			imgui_captured = false;
		}
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderPass* pass = new RenderPass;

		scene_manager.update(delta_time);
		scene_manager.render(pass);
		
		pass->do_render();

		
		imgui_shader->use();


		//
		//	Set fps limit
		//
		static float target_fps = 60.0f;
		ImGui::Begin("Game info");
		ImGui::Text("Deltatime: %.4f",delta_time);
		ImGui::Text("Target FPS: %.1f",target_fps);
		ImGui::SliderFloat("",&target_fps, 10.0f, 1000.0f, "%.1f");
		ImGui::End();

		imgui_shader->upload_projection(uicam->get_projection());
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
		

		if(target_fps >= 10.0f && target_fps < 9999.0f && delta_ms < (1.0f / target_fps)*1000.0f)
		{
			SDL_Delay((int)((1.0f / target_fps)*1000.0f - delta_ms));
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

