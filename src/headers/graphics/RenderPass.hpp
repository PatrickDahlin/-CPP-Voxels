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
	RenderPass(Camera* main_camera = nullptr);
	~RenderPass();

	void draw_model(Model* model, ShaderProgram* shader, Camera* c = nullptr);

	void switch_to_cam(Camera* cam);

	void do_instant_render(Model* model, ShaderProgram* shader, Camera* c = nullptr);

	void do_render();

protected:

	struct DrawCall {
		Model* model;
		ShaderProgram* shader;
		Camera* cam;
	};

	std::vector<DrawCall>		draw_calls;

	Camera*						main_camera;

};

#endif