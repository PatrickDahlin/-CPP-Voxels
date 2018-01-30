#ifndef _RENDERPASS_HPP_
#define _RENDERPASS_HPP_

class VertexArray;
class ShaderProgram;
class Material;
class Camera;

#include <vector>

class RenderPass {
public:
	RenderPass();
	~RenderPass();

	void draw_model(VertexArray* buf, ShaderProgram* shader, Material* mat, Camera* c);

	void do_render();

protected:

	struct DrawCall {
		VertexArray* buf;
		ShaderProgram* shader;
		Material* mat;
		Camera* cam;
	};

	std::vector<DrawCall>		draw_calls;

};

#endif