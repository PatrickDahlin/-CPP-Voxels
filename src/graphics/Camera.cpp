#include "graphics/Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <math.h>

Camera::Camera() :
update_proj(true),
projection_mat(),
update_view(true),
view_mat(),
position(0,0,0),
pitch(0),
yaw(0),
roll(0)
{
}

Camera::~Camera()
{}

glm::mat4 Camera::get_projection()
{
	if(update_proj)
		update_projection();

	update_proj = false;
	return projection_mat;
}

glm::vec3 Camera::get_euler()
{
	return glm::vec3(pitch, yaw, roll);
}

glm::vec3 Camera::get_forward()
{
	glm::vec4 res = glm::vec4(0,0,1,1) * get_view();
	return glm::vec3(res.x,res.y,res.z);
}

glm::vec3 Camera::get_up()
{
	glm::vec4 res = glm::vec4(0,1,0,1) * get_view();
	return glm::vec3(res.x,res.y,res.z);
}

glm::vec3 Camera::get_right()
{
	glm::vec4 res = glm::vec4(1,0,0,1) * get_view();
	return glm::vec3(res.x, res.y, res.z);
}

glm::mat4 Camera::get_view()
{
	if(update_view)
	{
		view_mat = glm::translate(glm::mat4(1.0f), position); 

		glm::vec3 up = glm::vec3(0,1,0);
		
		// yaw rotation
		view_mat = glm::rotate(view_mat, yaw, up);
		
		// rotate the pitch
		view_mat = glm::rotate(view_mat, pitch, glm::vec3(1,0,0));

		view_mat = glm::rotate(view_mat, roll, glm::vec3(0,0,1));

		// lastly get the matrix with inverse
		view_mat = glm::inverse(view_mat);
		
		update_view = false;
	}
	
	return view_mat;
}

glm::vec3 Camera::get_position()
{
	return position;
}

void Camera::translate(float x, float y, float z)
{
	position += glm::vec3(x,y,z);
	update_view = true;
}

void Camera::translate(glm::vec3 amount)
{
	position += amount;
	update_view = true;
}

void Camera::rotate(float x, float y, float z)
{
	pitch += x;
	yaw += y;
	roll += z;
	
	const float two_pi = 2 * 3.1415f;
	pitch = fmod(pitch, two_pi);
	yaw = fmod(yaw, two_pi);
	roll = fmod(roll, two_pi);

	update_view = true;
}

void Camera::set_position(float x, float y, float z)
{
	position = glm::vec3(x,y,z);
	update_view = true;
}

void Camera::set_position(glm::vec3 amount)
{
	set_position(amount.x, amount.y, amount.z);
}

void Camera::set_rotation(float x, float y, float z)
{
	pitch = x;
	yaw = y;
	roll = z;

	const float two_pi = 2 * 3.1415f;
	pitch = fmod(pitch, two_pi);
	yaw = fmod(yaw, two_pi);
	roll = fmod(roll, two_pi);

	update_view = true;
}

void Camera::set_rotation(glm::vec3 amount)
{
	set_rotation(amount.x, amount.y, amount.z);
}
