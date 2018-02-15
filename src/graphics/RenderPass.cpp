#include "core/Macros.hpp"
#include "graphics/Camera.hpp"
#include "graphics/RenderPass.hpp"
#include "graphics/Model.hpp"
#include "graphics/ShaderProgram.hpp"
#include "graphics/Material.hpp"

#include <glm/gtc/matrix_transform.hpp>

RenderPass::RenderPass()
{

}

RenderPass::~RenderPass()
{

}

void RenderPass::draw_model(Model* model, ShaderProgram* shader, Camera* c)
{
	DrawCall d;

	assert(model->get_material() != nullptr);
	
	d.model = model;
	d.shader = shader;
	d.cam = c;

	draw_calls.emplace_back(d);
}

float r = 0;

void RenderPass::do_render()
{
	for(auto it : draw_calls)
	{
		it.shader->use();

		it.shader->upload_projection(it.cam->get_projection());
		it.shader->upload_view(it.cam->get_view());
		
		it.shader->upload_model(it.model->transform.get_combined());
		
		assert(it.model->get_material() != nullptr);
		assert(it.model->get_material()->texture != nullptr);
		it.model->get_material()->texture->bind();

		it.model->draw();

	}

	//cout("There are ");
	//cout(draw_calls.size());
	//cout(" calls this frame\n");

	draw_calls.clear(); 
}