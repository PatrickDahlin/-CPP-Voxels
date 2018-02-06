#ifndef _RENDERPASS_HPP_
#define _RENDERPASS_HPP_

class Model;
class ShaderProgram;
class Camera;

#include <vector>

/*

	This class should really be renamed to RenderBatch

	Also camera should only be passed in to do_render since
	we want to be able to render the world with multiple cameras

*/

class RenderPass {
public:
	RenderPass();
	~RenderPass();

	void draw_model(Model* model, ShaderProgram* shader, Camera* c);

	void do_render();

protected:

	struct DrawCall {
		Model* model;
		ShaderProgram* shader;
		Camera* cam;
	};

	std::vector<DrawCall>		draw_calls;

};

#endif