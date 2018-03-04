#include "game/Game.hpp"
#include "core/Common.hpp"
#include "graphics/RenderPass.hpp"
#include "core/Time.hpp"

// Imgui stuff
#include "graphics/ImGuiRenderer.hpp"
#include "graphics/ImGuiHelper.hpp"

#include <iostream>
#include <algorithm> // for std::max

#include "graphics/Framebuffer.hpp"
#include "game/PerlinScene.hpp"

bool Game::running = true;
GameWindow* Game::game_window = nullptr;

Game::Game(GameWindow* window) :
input(window),
main_scene(nullptr)
{
	Game::game_window = window;
	printf("Setting up Game\n");
	
	input.show_cursor(false);
	input.set_lock_mouse(true);

	init_imgui(window->get_width(), window->get_height());

	framebuffer = new Framebuffer(game_window->get_width(), game_window->get_height());

	assert(Game::game_window);
}


Game::~Game()
{
	dispose_imgui();
}

void Game::load()
{
	// We need to tell scenemanager the size of the window before loading any new scenes
	// As they'll be called resize on when initializing
	scene_manager.resized_window(game_window->get_width(), game_window->get_height());
	
	// No need to delete this scene, it's deleted in scenemanager
	scene_manager.switch_to_scene(new MainScene(&input, &scene_manager));
	//scene_manager.switch_to_scene(new PerlinScene(&input, &scene_manager));

	glClearColor(0.3f, 0.5f, 0.8f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_SCISSOR_TEST);

}

// Window size in fbo size
void Game::window_resize(int width, int height)
{
	resize_imgui(width, height);

	scene_manager.resized_window(width,height);
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

		pre_render_imgui(delta_time);


		// Make sure to do window resize BEFORE starting rendering

		ImGui::Begin("WindowSettings");
		ImGui::Text("Resolution: %ix%i",game_window->get_width(),game_window->get_height());
		
		WindowSettings settings = game_window->get_settings(); // @Cleanup
		static bool b = false, f = false;
		bool updates = false;
		if(ImGui::Checkbox("Borderless",&b))
		{
			updates = true;
		}
		if(ImGui::Checkbox("Fullscreen",&f))
		{
			updates = true;
		}
		static int size[2] = {1280,720};

		ImGui::InputInt2("Request size",size, 0);
		if(ImGui::Button("Resize", ImVec2(100,25)))
		{
			settings.width = size[0];
			settings.height = size[1];
			updates = true;
		}
		ImGui::End();
		
		if(updates)
		{
			settings.fullscreen = f;
			settings.borderless = b;
			game_window->apply_settings(settings);
		}


		// @Temporary
		if(input.get_key(SDLK_ESCAPE))
			Game::quit();

		if(input.get_key(SDLK_l))
		{
			static bool locked = true;
			locked = !locked;
			input.set_lock_mouse(locked);
			input.show_cursor(true);//!locked
		}
		
		//
		//	When ImGui wants mouse or keyboard, 
		//  disable all other gameplay input
		//
		static bool imgui_captured = false;
		if(request_input_imgui())
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
		

		// Rendering begins here

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderPass pass;// = new RenderPass;

		scene_manager.update(delta_time);
		scene_manager.render(pass); // Give RenderPass all data needed to render this frame
		

		//
		//	Set fps limit
		//
		static float target_fps = 60.0f;
		ImGui::Begin("Game info");
		ImGui::Text("FPS: %.2f", (1.0f / delta_time));
		ImGui::Text("Deltatime: %.4f",delta_time);
		ImGui::Text("Target FPS: %.1f",target_fps);
		ImGui::SliderFloat("",&target_fps, 10.0f, 1000.0f, "%.1f");
		ImGui::End();

		//
		//	If window size changed, resize fbo
		//	This because we can't guarantee that resize_window is
		//	called outside of rendering...
		//
		if(framebuffer->get_width() != game_window->get_width()) // Compare against render-res
		{
			printf("Resizing fbo\n");
			framebuffer->dispose();
			delete framebuffer;
			framebuffer = new Framebuffer(game_window->get_width(), game_window->get_height());
			printf("Done fbo resize\n");
		}
		
		framebuffer->prepare_render();

		pass.do_render(); // This does the actual rendering to screen

		// Do imgui rendering
		post_render_imgui();

		framebuffer->end_render();

		// Since we might be rendering at a different resolution, set the target viewport for the fbo to be the real
		// context size of the window. In the case of borderless this'll be native size and fbo is virtual size
		glViewport(0,0, game_window->get_real_screen_width(), game_window->get_real_screen_height());
		render_framebuffer(framebuffer);
		glViewport(0,0,game_window->get_width(),game_window->get_height());

		// Lastly swap buffers and delete old renderpass
		game_window->swap_buffers();
		


		//
		// Frametime calculation
		//
		// Calculate how long processing took, we need to calculate how long we need to wait for fps limit
		auto delta = Clock::now() - last_frame;
		double delta_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(delta).count() / 1000000.0;
		
		target_fps = std::max(10.0f, target_fps);
		if(target_fps < 9999.0f && delta_ms < (1.0f / target_fps)*1000.0f)
		{
			SDL_Delay((int)((1.0f / target_fps)*1000.0f - delta_ms));
		}

		// Calculate the real deltatime for this frame including the fps-limit time
		delta = Clock::now() - last_frame;
		delta_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(delta).count() / 1000000.0;
		delta_time = (float)(delta_ms / 1000.0);
	}

	cleanup_framebuffers();

	delete framebuffer;

	scene_manager.dispose();
}

void Game::quit()
{
	running = false;
	printf("Reqested quit\n");
}

GameWindow* Game::get_window()
{
	return game_window;
}