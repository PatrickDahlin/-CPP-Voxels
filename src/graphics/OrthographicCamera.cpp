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
	projection_mat = glm::ortho(left, right, top, bottom);
}




