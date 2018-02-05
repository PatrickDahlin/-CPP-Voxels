#include "game/Game.hpp"
#include "core/Macros.hpp"
#include "graphics/RenderPass.hpp"
#include "core/Time.hpp"

bool 			Game::running = true;

Game::Game(GameWindow* window) :
game_window(window),
input(window),
main_scene(nullptr)
{
	input.show_cursor(false);
}

Game::~Game()
{}

void Game::load()
{
	main_scene = new MainScene(&input, &scene_manager);
	scene_manager.switch_to_scene(main_scene);

	glClearColor(0.3f, 0.5f, 0.8f, 1.0f);
	glEnable(GL_DEPTH_TEST);
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

		// @Temporary
		if(input.get_key(SDLK_ESCAPE) == KeyState::PRESSED)
			Game::quit();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderPass* pass = new RenderPass;

		scene_manager.update(delta_time);
		scene_manager.render(pass);
		
		pass->do_render();

		game_window->swap_buffers();
	
		delete pass;

		//printf("delta_time: %.4f\n",delta_time);
		
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

