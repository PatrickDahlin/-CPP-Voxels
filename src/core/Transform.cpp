#include "core/Transform.hpp"

#include <glm/gtc/matrix_transform.hpp>

Transform::Transform() 
{
	parent = nullptr;
	position = glm::vec3(0);
	euler_angles = glm::vec3(0);
	model_scale = glm::vec3(1,1,1);
	combined = glm::mat4(1.0f);
}

Transform::~Transform() {}

glm::mat4 Transform::get_combined()
{
	if(needs_mat_update)
	{
		combined = glm::translate(glm::mat4(1.0f), position);
		combined *= glm::rotate(glm::mat4(1.0f), glm::radians(euler_angles.x), glm::vec3(1,0,0));
		combined *= glm::rotate(glm::mat4(1.0f), glm::radians(euler_angles.y), glm::vec3(1,0,0));
		combined *= glm::rotate(glm::mat4(1.0f), glm::radians(euler_angles.z), glm::vec3(1,0,0));
		combined *= glm::scale(glm::mat4(1.0f), model_scale);
		
		needs_mat_update = false;
		return combined;
	}
	else
		return combined;
}

void Transform::translate(float x, float y, float z)
{
	position += glm::vec3(x, y, z);
	needs_mat_update = true;
}

void Transform::rotate(float x, float y, float z)
{
	euler_angles += glm::vec3(x, y, z);
	needs_mat_update = true;
}
void Transform::scale(float x, float y, float z)
{
	model_scale += glm::vec3(x, y, z);
	needs_mat_update = true;
}

void Transform::set_position(float x, float y, float z)
{
	position = glm::vec3(x, y, z);
	needs_mat_update = true;
}
void Transform::set_rotation(float x, float y, float z)
{
	euler_angles = glm::vec3(x, y, z);
	needs_mat_update = true;
}
void Transform::set_scale(float x, float y, float z)
{
	model_scale = glm::vec3(x, y, z);
	needs_mat_update = true;
}

void Transform::set_position(glm::vec3 pos)
{
	position = pos;
	needs_mat_update = true;
}

void Transform::set_rotation(glm::vec3 rot)
{
	euler_angles = rot;
	needs_mat_update = true;
}

void Transform::set_scale(glm::vec3 scale)
{
	model_scale = scale;
	needs_mat_update = true;
}

void Transform::set_uniform_scale(float scale)
{
	model_scale = glm::vec3(scale);
	needs_mat_update = true;
}