#include "../headers/core/Input.hpp"
#include "../headers/core/Macros.hpp"
#include <SDL2/SDL.h>
#include <glm/vec2.hpp>

Input::Input() : key_map(),
				 mouse_x(0),
				 mouse_y(0),
				 mouse_delta_x(0),
				 mouse_delta_y(0),
				 mouse_btn_state(),
				 scroll_delta(0)
{
}

Input::~Input()
{
}

void Input::set_lock_mouse(bool lock)
{
	if(lock)
		SDL_SetRelativeMouseMode(SDL_TRUE);
	else
		SDL_SetRelativeMouseMode(SDL_FALSE);
}

void Input::show_cursor(bool show)
{
	if(show)
		SDL_ShowCursor(1);
	else
		SDL_ShowCursor(0);
}

KeyState Input::get_key(SDL_Keycode key)
{
	return key_map[key];
}

KeyState Input::get_mouse_btn(unsigned short button)
{
	return mouse_btn_state[button];
}

glm::ivec2 Input::get_mouse_pos() const
{
	return glm::ivec2(mouse_x,mouse_y);
}

glm::ivec2 Input::get_mouse_pos_delta() const
{
	return glm::ivec2(mouse_delta_x, mouse_delta_y);
}

int Input::get_scroll_delta() const
{
	return scroll_delta;
}

void Input::poll_events()
{

	mouse_delta_x = 0;
	mouse_delta_y = 0;
	scroll_delta = 0;

	for (auto& it : key_map)
	{
		switch (it.second)
		{
		case KeyState::PRESSED:
			it.second = KeyState::REPEAT;
			break;
		case KeyState::RELEASED:
			it.second = KeyState::NONE;
			break;
		case KeyState::NONE:
		default:
			break;
		}
	}

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			key_map[event.key.keysym.sym] = KeyState::PRESSED;
			break;
		case SDL_KEYUP:
			key_map[event.key.keysym.sym] = KeyState::RELEASED;
			break;
		case SDL_QUIT:
			// OMG do stuff here
			exit(0); // this is NOT ok in the future, just temporarily here
			break;
		case SDL_MOUSEBUTTONDOWN:
			mouse_btn_state[event.button.button] = KeyState::PRESSED;
			break;
		case SDL_MOUSEBUTTONUP:
			mouse_btn_state[event.button.button] = KeyState::RELEASED;
			break;
		case SDL_MOUSEMOTION:
			mouse_x = event.motion.x;
			mouse_y = event.motion.y;
			mouse_delta_x = event.motion.xrel;
			mouse_delta_y = event.motion.yrel;
			break;
		case SDL_MOUSEWHEEL:
			// Scroll
			scroll_delta = event.wheel.y;
			break;
		case SDL_WINDOWEVENT:
			// Window resize
			break;
		default:
			break;
		}
	}
}