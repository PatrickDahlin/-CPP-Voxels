#ifndef _INPUT_HPP_
#define _INPUT_HPP_

#include <unordered_map>
#include <SDL2/SDL.h>

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
	Input();
	~Input();

	void poll_events();

	KeyState get_key(SDL_Keycode code);

	KeyState get_mouse_btn(unsigned short button);

	

	// @TODO SDL supports text input compositions which
	// make text inputting easier. implement this in the future
	void start_text_input();  // @unimplemented
	void stop_text_input();   // @unimplemented
	void get_inputted_text(); // @unimplemented

  private:
	std::unordered_map<SDL_Keycode, KeyState> key_map;
	int mouse_x;
	int mouse_y;
	int mouse_delta_x;
	int mouse_delta_y;

	std::unordered_map<int, KeyState> mouse_btn_state;
	int scroll_delta;
};

#endif