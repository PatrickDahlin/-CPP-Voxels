#ifndef _GAME_HPP_
#define _GAME_HPP_

#include "core/GameWindow.hpp"
#include "core/Input.hpp"
#include "core/SceneManager.hpp"
#include "game/MainScene.hpp"

class Framebuffer;

class Game
{
public:
	Game(const std::shared_ptr<GameWindow>& window);
	~Game();

	void load();

	void run();

	void window_resize(int width, int height);

	static void quit();

	static auto get_window();

protected:

	static bool				running;
	SceneManager			scene_manager;
	static std::shared_ptr<GameWindow> game_window;
	Input					input;

	std::unique_ptr<Framebuffer> 			framebuffer;

};

#endif