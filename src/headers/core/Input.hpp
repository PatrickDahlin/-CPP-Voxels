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

enum class GamePadAxis 
{
	LEFTX,
	LEFTY,
	RIGHTX,
	RIGHTY,
	TRIGGERLEFT,
	TRIGGERRIGHT,
	MAX,
	INVALID
};

enum class GamePadBtn
{
	A,
	B,
	X,
	Y,
	BACK,
	GUIDE,
	START,
	LEFTSTICK,
	RIGHTSTICK,
	LEFTSHOULDER,
	RIGHTSHOULDER,
	DPAD_UP,
	DPAD_DOWN,
	DPAD_LEFT,
	DPAD_RIGHT,
	MAX,
	INVALID
};

/*
	This class will handle inputs
*/
class Input
{
  public:
	Input(GameWindow* window);
	~Input();

	// Updates key state and mouse state
	void poll_events();

	// Returns the current state of given key
	KeyState get_key_state(SDL_Keycode code);

	// Returns true while key is pressed
	bool get_key_down(SDL_Keycode code);

	// Returns true the frame the key is pressed
	bool get_key(SDL_Keycode code);

	// Returns the state of given mouse button index
	KeyState get_mouse_btn_state(unsigned short button);

	// Returns true while key is pressed
	bool get_mouse_btn(unsigned short button);

	// Returns true the frame the key is pressed
	bool get_mouse_btn_down(unsigned short button);

	// Returns the mouse position
	glm::ivec2 get_mouse_pos() const;

	// Returns the mouse delta this frame
	glm::ivec2 get_mouse_pos_delta() const;

	// Returns the mouse scroll delta this frame
	int get_scroll_delta() const;


	//
	// Gamecontroller stuff
	//

	// @TODO multi-controller setup not implemented

	// Returns a value between -1.0 to 1.0
	float get_controller_axis(int controller, GamePadAxis axis);

	// Returns whether button is held for given controller
	bool get_controller_button(int controller, GamePadBtn button);

	//
	// End Gamecontroller
	//


	// Is input enabled?
	bool is_enabled() const;

	// Is the mouse locked to the center of the screen?
	bool is_mouse_locked() const;

	// Set whether mouse should be locked to the center of the screen
	void set_lock_mouse(bool lock);

	// Set whether cursor should be shown or not (only applies to mouse within the window)
	void show_cursor(bool show);

	// Sets the mouse position within the window
	void set_mouse_pos(const int x, const int y);

	// Sets whether input should be enabled or not
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
	bool window_focused;

	std::unordered_map<int, KeyState> mouse_btn_state;
	int scroll_delta;

	SDL_GameController*	controller;
};

#endif