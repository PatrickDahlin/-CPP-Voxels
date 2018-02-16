#include "game/DebugCamera.hpp"
#include "core/Input.hpp"
#include "core/Macros.hpp"
#include <algorithm>

DebugCamera::DebugCamera(float fov, int width, int height, float near, float far) : 
PerspectiveCamera(fov, width, height, near, far),
input(nullptr),
mouse_sensitivity(0.1),
fly_speed(5),
mouse_x_smoothing(0),
mouse_y_smoothing(0),
vertical(0),
horizontal(0),
mouse_input_active(true)
{
}

DebugCamera::~DebugCamera()
{}

void DebugCamera::set_mouse_input_active(const bool active)
{
	mouse_input_active = active;
}

bool DebugCamera::is_mouse_input_active() const {
	return mouse_input_active;
}

void DebugCamera::update(const float delta)
{
	if(input == nullptr || !input->is_enabled() || !mouse_input_active) return;

	if(input->get_key_down(SDLK_w))
	{
		translate(-get_forward() * fly_speed * delta);
	}
	else if(input->get_key_down(SDLK_s))
	{
		translate(get_forward() * fly_speed * delta);
	}

	if(input->get_key_down(SDLK_a))
	{
		translate(-get_right() * fly_speed * delta);
	}
	else if(input->get_key_down(SDLK_d))
	{
		translate(get_right() * fly_speed * delta);
	}

	if(input->get_key_down(SDLK_e))
	{
		translate(get_up() * fly_speed * delta);
	}
	else if(input->get_key_down(SDLK_q))
	{
		translate(-get_up() * fly_speed * delta);
	}

	// Gamepad support
	// @TODO add separate gamepad sensitivity values
	// 0.2f is used as a dead-space before stick does anything

	//
	// Rotation
	//
	float gpad_v = input->get_controller_axis(0, GamePadAxis::RIGHTY);
	float gpad_h = input->get_controller_axis(0, GamePadAxis::RIGHTX);

	if(abs(gpad_v) < 0.2f) 
	{
		gpad_v = 0;
	}	
	else
	{
		if(gpad_v < 0.0f)
			gpad_v = (gpad_v + 0.2f) / 0.8f;
		else
			gpad_v = (gpad_v - 0.2f) / 0.8f;
	}
	if(abs(gpad_h) < 0.2f)
	{
		gpad_h = 0;
	}
	else
	{
		if(gpad_h < 0.0f)
			gpad_h = (gpad_h + 0.2f) / 0.8f;
		else
			gpad_h = (gpad_h + 0.2f) / 0.8f;
	}

	
	gpad_v *= mouse_sensitivity * delta;
	gpad_h *= mouse_sensitivity * delta;

	rotate(-gpad_v, -gpad_h, 0);

	//
	// Movement
	//
	float gpad_f = input->get_controller_axis(0, GamePadAxis::LEFTY);
	float gpad_r = input->get_controller_axis(0, GamePadAxis::LEFTX);

	if(abs(gpad_f) < 0.2f) 
	{
		gpad_f = 0;
	}	
	else
	{
		if(gpad_f < 0.0f)
			gpad_f = (gpad_f + 0.2f) / 0.8f;
		else
			gpad_f = (gpad_f - 0.2f) / 0.8f;
	}
	if(abs(gpad_r) < 0.2f)
	{
		gpad_r = 0;
	}
	else
	{
		if(gpad_r < 0.0f)
			gpad_r = (gpad_r + 0.2f) / 0.8f;
		else
			gpad_r = (gpad_r + 0.2f) / 0.8f;
	}

	translate(get_right() * gpad_r * fly_speed * delta * 2.0f);
	translate(get_forward() * gpad_f * fly_speed * delta * 2.0f);


	//
	// Mouse control
	//
	mouse_x_smoothing += input->get_mouse_pos_delta().x;
	mouse_y_smoothing += input->get_mouse_pos_delta().y;

	float newdelta = std::max(0.001f, delta); // Limit the delta so we aren't going too low

	vertical = mouse_y_smoothing * 10.0f * newdelta;
	horizontal = mouse_x_smoothing * 10.0f * newdelta;

	mouse_y_smoothing -= vertical;
	mouse_x_smoothing -= horizontal;

	float v = -vertical * mouse_sensitivity * 0.001f;
	float h = -horizontal * mouse_sensitivity * 0.001f;

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