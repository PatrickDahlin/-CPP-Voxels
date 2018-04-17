#include "graphics/PerspectiveCamera.hpp"
#include <glm/gtc/matrix_transform.hpp>

PerspectiveCamera::	PerspectiveCamera(float fov, int width, int height, float near, float far) :
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

void PerspectiveCamera::update_frustum()
{
	/*assert(width > 0);
	assert(height > 0);
	float radfov = 3.1415f * (fov / 180.0f); 
	float aspect = (float)width / (float)height;
	// Size of near & far planes
	float hnear = 2 * tan(radfov / 2) * near;
	float wnear = hnear * aspect;


	vec3 forward = glm::normalize(get_forward());

	vec3 cnear = position + forward * near;
	vec3 cfar = position + forward * far;

	vec3 up = glm::normalize(get_up());
	vec3 right= glm::normalize(get_right());
	*/
	// frustum indices:
	// 0 near
	// 1 far
	// 2 top
	// 3 bottom
	// 4 left
	// 5 right
	
	mat4 combined = view_mat * projection_mat;//get_projection() * get_view();

	frustum[0].a = combined[3][0] + combined[2][0];
	frustum[0].b = combined[3][1] + combined[2][1];
	frustum[0].c = combined[3][2] + combined[2][2];
	frustum[0].d = combined[3][3] + combined[2][3];

	frustum[1].a = combined[3][0] - combined[2][0];
	frustum[1].b = combined[3][1] - combined[2][1];
	frustum[1].c = combined[3][2] - combined[2][2];
	frustum[1].d = combined[3][3] - combined[2][3];
	
	frustum[2].a = combined[3][0] - combined[1][0];
	frustum[2].b = combined[3][1] - combined[1][1];
	frustum[2].c = combined[3][2] - combined[1][2];
	frustum[2].d = combined[3][3] - combined[1][3];

	frustum[3].a = combined[3][0] + combined[1][0];
	frustum[3].b = combined[3][1] + combined[1][1];
	frustum[3].c = combined[3][2] + combined[1][2];
	frustum[3].d = combined[3][3] + combined[1][3];

	frustum[4].a = combined[3][0] + combined[0][0];
	frustum[4].b = combined[3][1] + combined[0][1];
	frustum[4].c = combined[3][2] + combined[0][2];
	frustum[4].d = combined[3][3] + combined[0][3];

	frustum[5].a = combined[3][0] - combined[0][0];
	frustum[5].b = combined[3][1] - combined[0][1];
	frustum[5].c = combined[3][2] - combined[0][2];
	frustum[5].d = combined[3][3] - combined[0][3];


	/*
	frustum[0] = Plane(cnear, forward);
	frustum[1] = Plane(cfar, -forward);
	
	// TOP
	vec3 a = (cnear + up * (hnear / 2)) - position;
	a = glm::normalize(glm::cross(a, right));
	frustum[2] = Plane(position, a);
	//frustum[2].p = cnear; frustum[2].n = forward;

	// Bottom
	a = (cnear - up * (hnear / 2)) - position;
	a = glm::normalize(glm::cross(right, a));
	frustum[3] = Plane(position, a);
	//frustum[3].p = cnear; frustum[3].n = forward;

	// LEFT
	a = (cnear - right * (wnear / 2)) - position;
	a = glm::normalize(glm::cross(a, up));
	frustum[4] = Plane(position, a);
	//frustum[4].p = cnear; frustum[4].n = forward;

	// RIGHT
	a = (cnear + right * (wnear / 2)) - position;
	a = glm::normalize(glm::cross(up, a));
	frustum[5] = Plane(position, a);
	//frustum[5].p = cnear; frustum[5].n = forward;
	*/
}

void PerspectiveCamera::update_projection()
{
	float radfov = 3.1415f * (fov / 180.0f); 
	float aspect = (float)width / (float)height;
	projection_mat = glm::perspective(radfov, aspect, near, far);	
	update_frustum();
}


