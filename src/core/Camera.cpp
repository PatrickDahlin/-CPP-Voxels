#include "../headers/core/Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float fov, int width, int height, float near, float far) :
update_proj(true),
projection_mat(),
update_view(true),
view_mat(),
fov(fov),
width(width),
height(height),
near(near),
far(far),
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
		projection_mat = glm::perspective(fov, ((float)width / (float)height), near, far);

	update_proj = false;
	return projection_mat;
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
	update_view = true;
}

void Camera::set_position(float x, float y, float z)
{
	position = glm::vec3(x,y,z);
	update_view = true;
}

void Camera::set_position(glm::vec3 amount)
{
	position = amount;
	update_view = true;
}

void Camera::set_rotation(float x, float y, float z)
{
	pitch = x;
	yaw = y;
	roll = z;
	update_view = true;
}

void Camera::set_rotation(glm::vec3 amount)
{
	pitch = amount.x;
	yaw = amount.y;
	roll = amount.z;
	update_view = true;
}
