#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include "Transform.hpp"

class Camera {
public:
	Camera(float fov, int width, int height, float near, float far);
	~Camera();

	void translate(float x, float y, float z);
	void rotate(float x, float y, float z);

	void set_position(float x, float y, float z);
	void set_rotation(float x, float y, float z);

	void set_fov(float fov);
	void resize(int width, int height);
	void set_near(float near);
	void set_far(float far);


	glm::mat4 get_projection();
	glm::mat4 get_view();

protected:

	bool			update_proj;
	glm::mat4		projection_mat;

	float			fov;
	int				width;
	int				height;
	float			near;
	float 			far;

	// Used for viewmatrix
	Transform		transform;
	
};

#endif


/*
draw front-to-back
all meshes with the same shader at once, so sort by shader




void render()
{

	*clear*


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

	*swapbuffers*
}
*/