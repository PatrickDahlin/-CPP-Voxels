#include "core/Input.hpp"
#include "core/Macros.hpp"
#include "core/GameWindow.hpp"
#include <SDL2/SDL.h>
#include <glm/vec2.hpp>

#include <iostream>
#include <imgui/imgui.h> // Not really liking the use of imgui here

Input::Input(GameWindow* window) : key_map(),
				 mouse_x(window->get_width()/2),
				 mouse_y(window->get_height()/2),
				 last_mouse_x(mouse_x),
				 last_mouse_y(mouse_y),
				 lock_mouse(true),
				 mouse_delta_x(0),
				 mouse_delta_y(0),
				 mouse_last_enabled_x(mouse_x),
				 mouse_last_enabled_y(mouse_y),
				 window(window),
				 enabled(true),
				 mouse_btn_state(),
				 scroll_delta(0)
{
	window->set_mouse_pos( window->get_width()/2, window->get_height()/2 );
}

Input::~Input()
{
}

void Input::set_lock_mouse(bool lock)
{
	lock_mouse = lock;
	if(!lock_mouse)
	{
		last_mouse_x = mouse_x;
		last_mouse_y = mouse_y;
	}
}

bool Input::is_mouse_locked() const{
	return lock_mouse;
}

void Input::set_mouse_pos(const int x, const int y)
{
	if(!lock_mouse)
		window->set_mouse_pos(x,y);
	else
		coutln("Tried moving mouse while it's locked");
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
	if(!enabled) return KeyState::NONE;
	return key_map[key];
}

KeyState Input::get_mouse_btn(unsigned short button)
{
	if(!enabled) return KeyState::NONE;

	if(button == 0)
		return mouse_btn_state[SDL_BUTTON_LEFT];
	else if(button == 1)
		return mouse_btn_state[SDL_BUTTON_RIGHT];
	else if(button == 2)
		return mouse_btn_state[SDL_BUTTON_MIDDLE];
	else
		return mouse_btn_state[button];
}

glm::ivec2 Input::get_mouse_pos() const
{
	if(!enabled) return glm::ivec2(mouse_last_enabled_x,mouse_last_enabled_y);
	return glm::ivec2(mouse_x,mouse_y);
}

glm::ivec2 Input::get_mouse_pos_delta() const
{
	if(!enabled) return glm::ivec2(0,0);
	return glm::ivec2(mouse_delta_x, mouse_delta_y);
}

int Input::get_scroll_delta() const
{
	if(!enabled) return 0;
	return scroll_delta;
}

bool Input::is_enabled() const
{
	return enabled;
}

void Input::set_input_enabled(bool ena)
{
	enabled = ena;
	mouse_last_enabled_x = mouse_x;
	mouse_last_enabled_y = mouse_y;
}

bool Input::get_key_down(SDL_Keycode code)
{
	if(get_key(code) == KeyState::PRESSED ||
		get_key(code) == KeyState::REPEAT)
		return true;

	return false;
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
	ImGuiIO& io = ImGui::GetIO();
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			//if(!enabled) break;
			key_map[event.key.keysym.sym] = KeyState::PRESSED;
			
			assert(event.key.keysym.scancode >= 0 && event.key.keysym.scancode < IM_ARRAYSIZE(io.KeysDown));
			io.KeysDown[event.key.keysym.scancode] = true;
			io.KeyShift = (SDL_GetModState() & KMOD_SHIFT) != 0;
			io.KeyCtrl = (SDL_GetModState() & KMOD_CTRL) != 0;
			io.KeyAlt = (SDL_GetModState() & KMOD_ALT) != 0;
			io.KeySuper = (SDL_GetModState() & KMOD_GUI) != 0;
			break;
		case SDL_KEYUP:
			//if(!enabled) break;
			key_map[event.key.keysym.sym] = KeyState::RELEASED;
			
			assert(event.key.keysym.scancode >= 0 && event.key.keysym.scancode < IM_ARRAYSIZE(io.KeysDown));
			io.KeysDown[event.key.keysym.scancode] = false;
			io.KeyShift = (SDL_GetModState() & KMOD_SHIFT) != 0;
			io.KeyCtrl = (SDL_GetModState() & KMOD_CTRL) != 0;
			io.KeyAlt = (SDL_GetModState() & KMOD_ALT) != 0;
			io.KeySuper = (SDL_GetModState() & KMOD_GUI) != 0;
			break;
		case SDL_QUIT:
			// @TODO OMG do stuff here
			exit(0); // this is NOT ok in the future, just temporarily here
			break;
		case SDL_MOUSEBUTTONDOWN:
			//if(!enabled) break;
			mouse_btn_state[event.button.button] = KeyState::PRESSED;
			
			if(event.button.button == SDL_BUTTON_LEFT) io.MouseDown[0] = true;
			if(event.button.button == SDL_BUTTON_RIGHT) io.MouseDown[1] = true;
			if(event.button.button == SDL_BUTTON_MIDDLE) io.MouseDown[2] = true;
			break;
		case SDL_MOUSEBUTTONUP:
			//if(!enabled) break;
			mouse_btn_state[event.button.button] = KeyState::RELEASED;
			
			if(event.button.button == SDL_BUTTON_LEFT) io.MouseDown[0] = false;
			if(event.button.button == SDL_BUTTON_RIGHT) io.MouseDown[1] = false;
			if(event.button.button == SDL_BUTTON_MIDDLE) io.MouseDown[2] = false;
			break;
		case SDL_MOUSEMOTION:
			//if(!enabled) break;
			mouse_x = event.motion.x;
			mouse_y = event.motion.y;
			io.MousePos = ImVec2(mouse_x, mouse_y);
			// These values can't be used for camera control
			// Seems like SDL somehow doesn't update xrel too often
			// and thusly causes stuttered motion for camera input
			//mouse_delta_x = event.motion.xrel;
			//mouse_delta_y = event.motion.yrel;
			
			break;
		case SDL_MOUSEWHEEL:
			//if(!enabled) break;
			// Scroll
			scroll_delta = event.wheel.y;
			
			if(event.wheel.x > 0) io.MouseWheelH +=1;
			if(event.wheel.x < 0) io.MouseWheelH -=1;
			if(event.wheel.y > 0) io.MouseWheel +=1;
			if(event.wheel.y < 0) io.MouseWheel -=1;
			
			break;
		case SDL_WINDOWEVENT:
			
			switch(event.window.event)
			{
				case SDL_WINDOWEVENT_FOCUS_LOST:
					set_input_enabled(false);
					break;
				case SDL_WINDOWEVENT_FOCUS_GAINED:
					set_input_enabled(true);
					break;
				default:
					break;
			}
			
			break;
		case SDL_TEXTINPUT:
			//if(!enabled) break;
			io.AddInputCharactersUTF8(event.text.text);
			break;
		default:
			break;
		}
	}

	// We don't want to cage the mouse while application is not in focus
	if(!enabled) {
		return;
	}

	mouse_delta_x = mouse_x - last_mouse_x;
	mouse_delta_y = mouse_y - last_mouse_y;
	
	if(!lock_mouse)
	{
		last_mouse_x = mouse_x;
		last_mouse_y = mouse_y;
	}
	else
	{
		window->set_mouse_pos( window->get_width()/2, window->get_height()/2 );
		last_mouse_x = window->get_width()/2;
		last_mouse_y = window->get_height()/2;
	}
}