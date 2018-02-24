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

	void window_resize(int width, int height);

	static void quit();

	static GameWindow* get_window();

protected:

	static bool				running;
	SceneManager			scene_manager;
	static GameWindow*		game_window;
	Input					input;
	MainScene*				main_scene;

};

#endif