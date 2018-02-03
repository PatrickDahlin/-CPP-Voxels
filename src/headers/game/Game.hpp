#ifndef _GAME_HPP_
#define _GAME_HPP_

#include "core/GameWindow.hpp"
#include "core/Input.hpp"
#include "core/SceneManager.hpp"
#include "game/MainScene.hpp"

class Game
{
public:
	Game(GameWindow* window);
	~Game();

	void load();

	void run();

	static void load_scene(Scene* scene);

	static void quit();

protected:

	static bool				running;
	static SceneManager		scene_manager;
	GameWindow*				game_window;
	Input					input;
	MainScene*				main_scene;

};

#endif