#ifndef _RENDERPASS_HPP_
#define _RENDERPASS_HPP_

class GLBuffer;
class ShaderProgram;
class Material;

#include <vector>

class RenderPass {
public:
	RenderPass();
	~RenderPass();

	void draw_model(GLBuffer* buf, ShaderProgram* shader, Material* mat);

	void do_render();

protected:

	struct DrawCall {
		GLBuffer* buf;
		ShaderProgram* shader;
		Material* mat;
	};

	std::vector<DrawCall>		draw_calls;

};

#endif