#ifndef _RENDERPASS_HPP_
#define _RENDERPASS_HPP_

class Model;
class ShaderProgram;
class Camera;

#include <vector>

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