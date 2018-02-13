#include "graphics/OrthographicCamera.hpp"
#include <glm/gtc/matrix_transform.hpp>


OrthographicCamera::OrthographicCamera(int left,
										int right,
										int top,
										int bottom) :
Camera(),
left(left),
right(right),
top(top),
bottom(bottom)
{}

OrthographicCamera::~OrthographicCamera()
{}

void OrthographicCamera::update_projection()
{
	// Be careful of using ints as it can create a matrix of integers, which won't work for projection matrix
	projection_mat = glm::ortho((float)left, (float)right, (float)bottom, (float)top, -1000.0f,1000.0f);
}




