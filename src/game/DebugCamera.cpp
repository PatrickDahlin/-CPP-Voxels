#include "game/DebugCamera.hpp"
#include "core/Input.hpp"
#include "core/Macros.hpp"

DebugCamera::DebugCamera(float fov, int width, int height, float near, float far) : 
PerspectiveCamera(fov, width, height, near, far),
input(nullptr),
mouse_sensitivity(0.1),
fly_speed(5),
mouse_x_smoothing(0),
mouse_y_smoothing(0),
vertical(0),
horizontal(0)
{
}

DebugCamera::~DebugCamera()
{}

void DebugCamera::update(const float delta)
{
	if(input == nullptr) return;

	if(input->get_key(SDLK_w) == KeyState::PRESSED ||
		input->get_key(SDLK_w) == KeyState::REPEAT)
	{
		translate(-get_forward() * fly_speed * delta);
	}
	else if(input->get_key(SDLK_s) == KeyState::PRESSED ||
			input->get_key(SDLK_s) == KeyState::REPEAT)
	{
		translate(get_forward() * fly_speed * delta);
	}

	if(input->get_key(SDLK_a) == KeyState::PRESSED ||
		input->get_key(SDLK_a) == KeyState::REPEAT)
	{
		translate(-get_right() * fly_speed * delta);
	}
	else if(input->get_key(SDLK_d) == KeyState::PRESSED ||
			input->get_key(SDLK_d) == KeyState::REPEAT)
	{
		translate(get_right() * fly_speed * delta);
	}

	// Smooth out mouse movements
	
	
	mouse_x_smoothing += input->get_mouse_pos_delta().x;
	mouse_y_smoothing += input->get_mouse_pos_delta().y;

	vertical = mouse_y_smoothing * 0.45f;
	horizontal = mouse_x_smoothing * 0.45f;

	mouse_y_smoothing -= vertical;
	mouse_x_smoothing -= horizontal;

	float v = -vertical * delta * mouse_sensitivity;
	float h = -horizontal * delta * mouse_sensitivity;

	rotate(v, h, 0);

}


void DebugCamera::set_input(Input* input)
{
	this->input = input;
}

void DebugCamera::set_mouse_sensitivity(const float val)
{
	mouse_sensitivity = val;
}

void DebugCamera::set_fly_speed(const float speed)
{
	fly_speed = speed;
}