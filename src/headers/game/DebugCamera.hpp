#ifndef _DebugCamera_HPP_
#define _DebugCamera_HPP_

#include "graphics/PerspectiveCamera.hpp"
class Input;

class DebugCamera : public PerspectiveCamera 
{
public:
	DebugCamera(float fov, int width, int height, float near, float far);
	~DebugCamera();

	void update(const float delta);

	void set_input(Input* input);

	void set_mouse_sensitivity(const float val);

	void set_fly_speed(const float speed);

	void set_mouse_input_active(const bool active);

	bool is_mouse_input_active() const;

protected:

	Input*			input;
	float			mouse_sensitivity;
	float			fly_speed;

	float			mouse_x_smoothing;
	float			mouse_y_smoothing;

	float			vertical;
	float			horizontal;

	bool			mouse_input_active;
};

#endif