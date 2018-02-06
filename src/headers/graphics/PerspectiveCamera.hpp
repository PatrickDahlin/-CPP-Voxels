#ifndef _PERSPECTIVECAM_HPP_
#define _PERSPECTIVECAM_HPP_

#include "graphics/Camera.hpp"

class PerspectiveCamera : public Camera 
{
public:
    PerspectiveCamera(float fov, int width, int height, float near, float far);
    ~PerspectiveCamera();

    
	void set_fov(float fov);
	void resize(int width, int height);
	void set_near(float near);
	void set_far(float far);


protected:

    void update_projection() override;

    float			fov;
	int				width;
	int				height;
	float			near;
	float 			far;
};

#endif