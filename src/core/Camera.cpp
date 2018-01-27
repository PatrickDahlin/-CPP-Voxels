#include "../headers/core/Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float fov, int width, int height, float near, float far) :
update_proj(true),
projection_mat(),
fov(fov),
width(width),
height(height),
near(near),
far(far),
transform()
{
	projection_mat = glm::perspectiveFov(fov, (float)width, (float)height, near, far);
	update_proj = false;
}

Camera::~Camera()
{}

glm::mat4 Camera::get_projection()
{
	if(update_proj)
		projection_mat = glm::perspectiveFov(fov, (float)width, (float)height, near, far);

	update_proj = false;
	return projection_mat;
}

glm::mat4 Camera::get_view()
{
	return transform.get_combined();
}

void Camera::set_fov(float fov)
{
	this->fov = fov;
	update_proj = true;
}

void Camera::set_near(float near)
{
	this->near = near;
	update_proj = true;
}

void Camera::set_far(float far)
{
	this->far = far;
	update_proj = true;
}

void Camera::resize(int width, int height)
{
	this->width = width;
	this->height = height;
	update_proj = true;
}

void Camera::translate(float x, float y, float z)
{
	transform.translate(x,y,z);
}

void Camera::rotate(float x, float y, float z)
{
	transform.rotate(x,y,z);
}

void Camera::set_position(float x, float y, float z)
{
	transform.set_position(x,y,z);
}

void Camera::set_rotation(float x, float y, float z)
{
	transform.set_rotation(x,y,z);
}

