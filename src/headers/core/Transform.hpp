#ifndef _TRANSFORM_HPP_
#define _TRANSFORM_HPP_

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include "core/Common.hpp"

class Transform
{
public:
	Transform();
	~Transform();

	mat4 get_combined();

	void translate(float x, float y, float z);
	void rotate(float x, float y, float z);
	void scale(float x, float y, float z);

	void set_position(float x, float y, float z);
	void set_position(vec3 pos);
	void set_rotation(float x, float y, float z);
	void set_rotation(vec3 rot);
	void set_scale(float x, float y, float z);
	void set_scale(vec3 scale);
	void set_uniform_scale(float scale);

	vec3 get_position();
	vec3 get_euler();
	vec3 get_scale();

private:

	Transform* parent;

	bool needs_mat_update = true;
	mat4 combined;
	vec3 position;
	vec3 euler_angles;
	vec3 model_scale;
	
};

#endif