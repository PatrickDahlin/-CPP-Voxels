#ifndef _ORTHOGRAPHICCAM_HPP_
#define _ORTHOGRAPHICCAM_HPP_

#include "graphics/Camera.hpp"

class OrthographicCamera : public Camera {
public:
	OrthographicCamera(int left, int right, int top, int bottom);
	~OrthographicCamera();

protected:

    void update_projection() override;
	void update_frustum() override;

    int			left, right, top, bottom;
	
};

#endif