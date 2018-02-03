#include "game/Game.hpp"
#include "core/Macros.hpp"
#include "graphics/RenderPass.hpp"

bool Game::running = true;
SceneManager Game::scene_manager;

Game::Game(GameWindow* window) :
game_window(window),
input(),
main_scene(nullptr)
{
	input.set_lock_mouse(true);
	input.show_cursor(false);
}

Game::~Game()
{}

void Game::load()
{
	main_scene = new MainScene();
	scene_manager.switch_to_scene(main_scene);
}

void Game::run()
{
	unsigned int last_frame = SDL_GetTicks();
	float delta_time = 0.0f;

	while(running)
	{
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
		last_frame = SDL_GetTicks();

		if(delta_ms < 16)
			SDL_Delay(16 - delta_ms);
	}

	printf("Quitting\n");

	scene_manager.dispose();
}

void Game::load_scene(Scene* scene)
{
	scene_manager.switch_to_scene(scene);
}

void Game::quit()
{
	running = false;

	coutln("Reqested quit");
}



/*
void Game::crash(const char* msg)
{
	coutln("----------------------------------");
	coutln("Unhandled fatal error!");
	coutln("----------------------------------");
	coutln(msg);
	coutln("----------------------------------");

	exit(0);
}

void Game::assert(int v, const char* msg)
{
	if(!v)
	{
		coutln("----------------------------------");
		coutln("Assertion failed!");
		coutln("----------------------------------");
		coutln(msg);
		coutln("----------------------------------");
	}
}
*/