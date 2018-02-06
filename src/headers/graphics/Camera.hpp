#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include "core/Transform.hpp"

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

	glm::mat4 get_projection();
	glm::mat4 get_view();

	glm::vec3 get_forward();
	glm::vec3 get_up();
	glm::vec3 get_right();

protected:

	virtual void update_projection() = 0;

	bool			update_proj;
	glm::mat4		projection_mat;

	bool			update_view;
	glm::mat4		view_mat;

	glm::vec3		position;
	float			pitch, yaw, roll;
	
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