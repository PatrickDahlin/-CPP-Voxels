#include "../headers/core/RenderPass.hpp"
#include "../headers/core/Macros.hpp"
#include "../headers/core/VertexArray.hpp"
#include "../headers/core/ShaderProgram.hpp"
#include "../headers/core/Material.hpp"
#include "../headers/core/Camera.hpp"



RenderPass::RenderPass()
{

}

RenderPass::~RenderPass()
{

}

void RenderPass::draw_model(VertexArray* buf, ShaderProgram* shader, Material* mat, Camera* c)
{
	DrawCall d;

	d.buf = buf;
	d.shader = shader;
	d.mat = mat;
	d.cam = c;

	draw_calls.emplace_back(d);
}

void RenderPass::do_render()
{
	for(auto it : draw_calls)
	{
		it.shader->use();

		it.shader->upload_projection(it.cam->get_projection());
		it.shader->upload_view(it.cam->get_view());
		it.shader->upload_model(glm::mat4(1.0f));
		it.buf->draw();
	}

	cout("There are ");
	cout(draw_calls.size());
	cout(" calls this frame\n");

	draw_calls.clear(); 
}