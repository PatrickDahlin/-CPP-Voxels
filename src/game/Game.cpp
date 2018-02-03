#include "game/Game.hpp"
#include "core/Macros.hpp"
#include "graphics/RenderPass.hpp"

bool 			Game::running = true;

Game::Game(GameWindow* window) :
game_window(window),
main_scene(nullptr)
{
	input.set_lock_mouse(true);
	input.show_cursor(false);
}

Game::~Game()
{}

void Game::load()
{
	main_scene = new MainScene(&input, &scene_manager);
	scene_manager.switch_to_scene(main_scene);
}

void Game::run()
{
	unsigned int last_frame = SDL_GetTicks();
	float delta_time = 0.0f;

	while(running)
	{
		last_frame = SDL_GetTicks();

		input.poll_events();

		// @Temporary
		if(input.get_key(SDLK_ESCAPE) == KeyState::PRESSED)
			Game::quit();

		RenderPass* pass = new RenderPass;

		scene_manager.update(delta_time);
		scene_manager.render(pass);
		
		pass->do_render();

		game_window->swap_buffers();
	
		delete pass;


		//
		// Frametime calculation
		//
		unsigned int delta_ms = SDL_GetTicks() - last_frame;
		delta_time = (float)( delta_ms / 1000.0f );
		

		printf("deltams:%i\n",delta_ms);

		if(delta_ms < 16)
		{
			printf("sleep for: %i\n", 16-delta_ms);
			SDL_Delay(16 - delta_ms);
		}
	}

	scene_manager.dispose();
}

void Game::quit()
{
	running = false;

	coutln("Reqested quit");
}

