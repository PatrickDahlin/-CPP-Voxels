#ifndef _TRANSFORM_HPP_
#define _TRANSFORM_HPP_

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Transform
{
public:
	Transform();
	~Transform();

	glm::mat4 get_combined();

	void translate(float x, float y, float z);
	void rotate(float x, float y, float z);
	void scale(float x, float y, float z);

	void set_position(float x, float y, float z);
	void set_position(glm::vec3 pos);
	void set_rotation(float x, float y, float z);
	void set_rotation(glm::vec3 rot);
	void set_scale(float x, float y, float z);
	void set_scale(glm::vec3 scale);
	void set_uniform_scale(float scale);

private:

	Transform* parent;

	bool needs_mat_update = true;
	glm::mat4 combined;
	glm::vec3 position;
	glm::vec3 euler_angles;
	glm::vec3 model_scale;
	
};

#endif