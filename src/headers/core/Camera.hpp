#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

class Camera {
public:
	Camera(float fov, int width, int height);
	~Camera();


protected:

	glm::mat4		combined;
	
	glm::vec3		position;
	glm::vec3		euler_angles;

};

#endif