#include "graphics/OrthographicCamera.hpp"
#include <glm/gtc/matrix_transform.hpp>


OrthographicCamera::OrthographicCamera(int left,
										int right,
										int top,
										int bottom) :
left(left),
right(right),
top(top),
bottom(bottom)
{}

OrthographicCamera::~OrthographicCamera()
{}

void OrthographicCamera::update_frustum()
{
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
	float near = -1000.0f;
	float far = 1000.0f;

	vec3 forward = glm::normalize(get_forward());
	vec3 up = glm::normalize(get_up());
	vec3 v_right= glm::normalize(get_right());


	vec3 cnear = position + forward * near;
	vec3 cfar = position + forward * far;

	// frustum indices:
	// 0 front (near plane)
	// 1 back (far plane)
	// 2 top
	// 3 bottom
	// 4 left
	// 5 right
	
	frustum[0].p = cnear; frustum[0].n = forward;
	frustum[1].p = cfar; frustum[1].n = -forward;

	// top
	frustum[2].p = cnear + (up * (float)top); frustum[2].n = -up; 
	// bottom
	frustum[3].p = cnear - (up * (float)bottom); frustum[3].n = up;
	// left
	frustum[4].p = cnear - (v_right * (float)left); frustum[4].n = v_right;
	//right
	frustum[5].p = cnear + (v_right * (float)right); frustum[5].n = -v_right;

	*/
}

void OrthographicCamera::update_projection()
{
	float near = -1000.0f;
	float far = 1000.0f;
	// Be careful of using ints as it can create a matrix of integers, which won't work for projection matrix
	projection_mat = glm::ortho((float)left, (float)right, (float)bottom, (float)top, near,far);
}




