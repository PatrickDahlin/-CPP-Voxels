#include "graphics/PerspectiveCamera.hpp"
#include <glm/gtc/matrix_transform.hpp>

PerspectiveCamera::	PerspectiveCamera(float fov, int width, int height, float near, float far) :
Camera(),
fov(fov),
width(width),
height(height),
near(near),
far(far)
{
}

PerspectiveCamera::~PerspectiveCamera()
{
}

void PerspectiveCamera::set_fov(float fov)
{
	this->fov = fov;
	update_proj = true;
}

void PerspectiveCamera::set_near(float near)
{
	this->near = near;
	update_proj = true;
}

void PerspectiveCamera::set_far(float far)
{
	this->far = far;
	update_proj = true;
}

void PerspectiveCamera::resize(int width, int height)
{
	this->width = width;
	this->height = height;
	update_proj = true;
}


void PerspectiveCamera::update_projection()
{
	projection_mat = glm::perspective(3.1415f * (fov / 180.0f), ((float)width / (float)height), near, far);
}


