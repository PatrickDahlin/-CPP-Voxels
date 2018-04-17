#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include "core/Common.hpp"
#include "core/Transform.hpp"
#include "graphics/AABB.hpp"

struct Plane {
	float a,b,c,d;
	float distance(vec3 other)
	{
		return a*other.x + b*other.y + c*other.z + d;
	}
	/*Plane() : p(0), n(0,1,0) {}
	Plane(vec3 p, vec3 n) : p(p), n(n) {
		d = -(glm::dot(n, p));
	}
	vec3 p;
	vec3 n;
	float d;

	float distance(vec3 other)
	{
		return (d + glm::dot(n, other));
	}*/
};

class Camera {
public:
	Camera();
	virtual ~Camera();

	void translate(float x, float y, float z);
	void translate(glm::vec3 vec);
	void rotate(float x, float y, float z);

	void set_position(float x, float y, float z);
	void set_position(glm::vec3 vec);
	void set_rotation(float x, float y, float z);
	void set_rotation(glm::vec3 vec);

	glm::vec3 get_position();
	glm::vec3 get_euler();
	glm::mat4 get_projection();
	glm::mat4 get_view();

	glm::vec3 get_forward();
	glm::vec3 get_up();
	glm::vec3 get_right();

	bool inside_frustum(AABB aabb);

protected:

	virtual void update_projection() = 0;
	virtual void update_frustum() = 0;

	bool			update_proj;
	glm::mat4		projection_mat;

	bool			update_view;
	glm::mat4		view_mat;

	glm::vec3		position;
	float			pitch, yaw, roll;

	Plane			frustum[6];
	
};

#endif


/*
draw front-to-back
all meshes with the same shader at once, so sort by shader


	renderpass pass
	
	Option 1:
		Collect all renderdata in pass object and do all rendering at end of frame

	renderobject:
		shader*
		camera*
		material
		model

	transparent objects need their own renderobject and are drawn back-to-front, regardless of shaders

	opaque front-to-back because of z-testing, reduces overdraw perhaps?


*/