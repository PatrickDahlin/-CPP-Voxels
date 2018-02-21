#include "core/Input.hpp"
#include "core/Common.hpp"
#include "core/GameWindow.hpp"
#include <SDL2/SDL.h>
#include <glm/vec2.hpp>

#include <iostream>
#include <imgui/imgui.h> // Not really liking the use of imgui here
#include <algorithm>

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
				 window_focused(true),
				 mouse_btn_state(),
				 scroll_delta(0),
				 controller(nullptr)
{
	window->set_mouse_pos( window->get_width()/2, window->get_height()/2 );
	
}

Input::~Input()
{
	//if(controller != nullptr)
	//	SDL_GameControllerClose(controller);
	
	controller = nullptr;
	window = nullptr;
	enabled = false;
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
		printf("Tried moving mouse while it's locked\n");
}

void Input::show_cursor(bool show)
{
	if(show)
		SDL_ShowCursor(1);
	else
		SDL_ShowCursor(0);
}

KeyState Input::get_key_state(SDL_Keycode key)
{
	if(!enabled) return KeyState::NONE;
	return key_map[key];
}

bool Input::get_mouse_btn_down(unsigned short button)
{
	return get_mouse_btn_state(button) == KeyState::PRESSED;
}

bool Input::get_mouse_btn(unsigned short button)
{
	return get_mouse_btn_state(button) == KeyState::PRESSED ||
			get_mouse_btn_state(button) == KeyState::REPEAT;
}

KeyState Input::get_mouse_btn_state(unsigned short button)
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


bool Input::get_key(SDL_Keycode code)
{
	return get_key_state(code) == KeyState::PRESSED;
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
	if(get_key_state(code) == KeyState::PRESSED ||
		get_key_state(code) == KeyState::REPEAT)
		return true;

	return false;
}

float Input::get_controller_axis(int contrlr, GamePadAxis axis)
{
	if(!enabled || controller == nullptr) return 0;//if(!enabled || controller == nullptr || !SDL_GameControllerGetAttached(controller)) return 0;
	if(SDL_GameControllerGetAttached(controller) == SDL_FALSE) return 0;
	
	SDL_GameControllerAxis ctrl_axis;
	switch(axis)
	{
		case GamePadAxis::LEFTX: ctrl_axis = SDL_CONTROLLER_AXIS_LEFTX; break;
		case GamePadAxis::LEFTY: ctrl_axis = SDL_CONTROLLER_AXIS_LEFTY; break;
		case GamePadAxis::RIGHTX: ctrl_axis = SDL_CONTROLLER_AXIS_RIGHTX; break;
		case GamePadAxis::RIGHTY: ctrl_axis = SDL_CONTROLLER_AXIS_RIGHTY; break;
		case GamePadAxis::TRIGGERLEFT: ctrl_axis = SDL_CONTROLLER_AXIS_TRIGGERLEFT; break;
		case GamePadAxis::TRIGGERRIGHT: ctrl_axis = SDL_CONTROLLER_AXIS_TRIGGERRIGHT; break;
		case GamePadAxis::MAX: ctrl_axis = SDL_CONTROLLER_AXIS_MAX; break;
		default: ctrl_axis = SDL_CONTROLLER_AXIS_INVALID; return 0;
	}
	return (float)SDL_GameControllerGetAxis(controller, ctrl_axis) / 32768.0f;
}

bool Input::get_controller_button(int contrlr, GamePadBtn button)
{
	if(!enabled || controller == nullptr || !SDL_GameControllerGetAttached(controller)) return false;
	SDL_GameControllerButton btn;
	switch(button)
	{
		case GamePadBtn::A: btn = SDL_CONTROLLER_BUTTON_A; break;
		case GamePadBtn::B: btn = SDL_CONTROLLER_BUTTON_B; break;
		case GamePadBtn::X: btn = SDL_CONTROLLER_BUTTON_X; break;
		case GamePadBtn::Y: btn = SDL_CONTROLLER_BUTTON_Y; break;
		case GamePadBtn::BACK: btn = SDL_CONTROLLER_BUTTON_BACK; break;
		case GamePadBtn::GUIDE: btn = SDL_CONTROLLER_BUTTON_GUIDE; break;
		case GamePadBtn::START: btn = SDL_CONTROLLER_BUTTON_START; break;
		case GamePadBtn::LEFTSTICK: btn = SDL_CONTROLLER_BUTTON_LEFTSTICK; break;
		case GamePadBtn::RIGHTSTICK: btn = SDL_CONTROLLER_BUTTON_RIGHTSTICK; break;
		case GamePadBtn::LEFTSHOULDER: btn = SDL_CONTROLLER_BUTTON_LEFTSHOULDER; break;
		case GamePadBtn::RIGHTSHOULDER: btn = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER; break;
		case GamePadBtn::DPAD_UP: btn = SDL_CONTROLLER_BUTTON_DPAD_UP; break;
		case GamePadBtn::DPAD_DOWN: btn = SDL_CONTROLLER_BUTTON_DPAD_DOWN; break;
		case GamePadBtn::DPAD_LEFT: btn = SDL_CONTROLLER_BUTTON_DPAD_LEFT; break;
		case GamePadBtn::DPAD_RIGHT: btn = SDL_CONTROLLER_BUTTON_DPAD_RIGHT; break;
		case GamePadBtn::MAX: btn = SDL_CONTROLLER_BUTTON_MAX; break;
		default: btn = SDL_CONTROLLER_BUTTON_INVALID; break;
	}
	return SDL_GameControllerGetButton(controller, btn) == 1;
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
			if(!window_focused) break;
			key_map[event.key.keysym.sym] = KeyState::PRESSED;
			
			assert(event.key.keysym.scancode >= 0 && event.key.keysym.scancode < IM_ARRAYSIZE(io.KeysDown));
			io.KeysDown[event.key.keysym.scancode] = true;
			io.KeyShift = (SDL_GetModState() & KMOD_SHIFT) != 0;
			io.KeyCtrl = (SDL_GetModState() & KMOD_CTRL) != 0;
			io.KeyAlt = (SDL_GetModState() & KMOD_ALT) != 0;
			io.KeySuper = (SDL_GetModState() & KMOD_GUI) != 0;
			break;
		case SDL_KEYUP:
			if(!window_focused) break;
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
			if(!window_focused) break;
			mouse_btn_state[event.button.button] = KeyState::PRESSED;
			
			if(event.button.button == SDL_BUTTON_LEFT) io.MouseDown[0] = true;
			if(event.button.button == SDL_BUTTON_RIGHT) io.MouseDown[1] = true;
			if(event.button.button == SDL_BUTTON_MIDDLE) io.MouseDown[2] = true;
			break;
		case SDL_MOUSEBUTTONUP:
			if(!window_focused) break;
			mouse_btn_state[event.button.button] = KeyState::RELEASED;
			
			if(event.button.button == SDL_BUTTON_LEFT) io.MouseDown[0] = false;
			if(event.button.button == SDL_BUTTON_RIGHT) io.MouseDown[1] = false;
			if(event.button.button == SDL_BUTTON_MIDDLE) io.MouseDown[2] = false;
			break;
		case SDL_JOYDEVICEADDED:
			printf("Controller add event\n");
			if(controller != nullptr) break;
			printf("Has no controller connected so we aquire one\n");
			// Look for first avaliable game controller
			for(int i = 0; i < SDL_NumJoysticks(); ++i)
			{
				if(SDL_IsGameController(i)){
					controller = SDL_GameControllerOpen(i);
					if(controller) break;
				}
			}
			break;
		case SDL_JOYDEVICEREMOVED:
			printf("Controller remove event\n");
			if(controller == nullptr) break;
			printf("Has no controller connected so we aquire one\n");
			// If we disconnected this joystick, find another one if avaliable
			controller = nullptr;
			for(int i = 0; i < SDL_NumJoysticks(); ++i)
			{
				if(SDL_IsGameController(i)){
					controller = SDL_GameControllerOpen(i);
					if(controller) break;
				}
			}
			break;
		case SDL_MOUSEMOTION:
			if(!window_focused) break;
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
			if(!window_focused) break;
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
				case SDL_WINDOWEVENT_ENTER:
					break;
				case SDL_WINDOWEVENT_LEAVE:
					break;
				case SDL_WINDOWEVENT_EXPOSED:
					break;
				case SDL_WINDOWEVENT_HIDDEN:
					break;
				case SDL_WINDOWEVENT_MAXIMIZED:
					break;
				case SDL_WINDOWEVENT_MINIMIZED:
					break;
				case SDL_WINDOWEVENT_RESTORED:
					break;
				case SDL_WINDOWEVENT_SHOWN:
					break;
				case SDL_WINDOWEVENT_TAKE_FOCUS:
					break;
				case SDL_WINDOWEVENT_FOCUS_LOST:
					window_focused = false;
					break;
				case SDL_WINDOWEVENT_FOCUS_GAINED:				
					window_focused = true;
					last_mouse_x = mouse_x;
					last_mouse_y = mouse_y;
					break;
				default:
					break;
			}
			
			break;
		case SDL_TEXTINPUT:
			if(!window_focused) break;
			io.AddInputCharactersUTF8(event.text.text);
			break;
		default:
			break;
		}
	}

	// We don't want to cage the mouse while application is not in focus
	if(!enabled || !window_focused) {
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