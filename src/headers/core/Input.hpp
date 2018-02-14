#ifndef _INPUT_HPP_
#define _INPUT_HPP_

#include <unordered_map>
#include <SDL2/SDL.h>
#include <glm/fwd.hpp>

class GameWindow;

// States for each key
// PRESSED = the frame the key was pressed
// REPEAT = each subsequent frame after first one, indicating a key hold
// RELEASED = the frame the key was released
enum class KeyState
{
	NONE,
	PRESSED,
	REPEAT,
	RELEASED
};

/*
	This class will handle inputs
*/
class Input
{
  public:
	Input(GameWindow* window);
	~Input();

	void poll_events();

	KeyState get_key(SDL_Keycode code);

	bool get_key_down(SDL_Keycode code);

	KeyState get_mouse_btn(unsigned short button);

	glm::ivec2 get_mouse_pos() const;

	glm::ivec2 get_mouse_pos_delta() const;

	int get_scroll_delta() const;

	bool is_enabled() const;

	bool is_mouse_locked() const;

	void set_lock_mouse(bool lock);

	void show_cursor(bool show);

	void set_mouse_pos(const int x, const int y);

	void set_input_enabled(bool enabled);
	
	// @TODO SDL supports text input compositions which
	// make text inputting easier. implement this in the future
	void start_text_input();  // @unimplemented
	void stop_text_input();   // @unimplemented
	void get_inputted_text(); // @unimplemented

  private:
	std::unordered_map<SDL_Keycode, KeyState> key_map;
	int mouse_x;
	int mouse_y;
	int last_mouse_x;
	int last_mouse_y;
	bool lock_mouse;
	int mouse_delta_x;
	int mouse_delta_y;
	int mouse_last_enabled_x;
	int mouse_last_enabled_y;
	GameWindow* window;
	bool enabled;

	std::unordered_map<int, KeyState> mouse_btn_state;
	int scroll_delta;
};

#endif