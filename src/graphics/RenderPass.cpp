#include "core/Common.hpp"
#include "graphics/Camera.hpp"
#include "graphics/RenderPass.hpp"
#include "graphics/Model.hpp"
#include "graphics/ShaderProgram.hpp"
#include "graphics/Material.hpp"
#include "graphics/VertexArray.hpp"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>

RenderPass::RenderPass(Camera* main_cam) : main_camera(main_cam)
{
}

RenderPass::~RenderPass()
{

}

void RenderPass::draw_model(Model* model, ShaderProgram* shader, Camera* c /*= nullptr*/)
{
	DrawCall d;

	assert(model->get_material() != nullptr);
	
	d.model = model;
	d.shader = shader;

	if(c) d.cam = c; else d.cam = main_camera;

	draw_calls.emplace_back(d);
}

void RenderPass::switch_to_cam(Camera* cam)
{
	main_camera = cam;
}


void RenderPass::do_instant_render(Model* model, ShaderProgram* shader, Camera* c /*= nullptr*/)
{
	assert(shader);
	assert(model);
	assert(c);
	
	shader->use();

	shader->upload_projection(c->get_projection());
	shader->upload_view(c->get_view());
	shader->upload_model(model->transform.get_combined());

	model->get_material()->texture->bind();

	model->draw();

}

void RenderPass::do_render()
{
	int v_c = 0;
	int t_c = 0;
	assert(main_camera);
	for(auto it : draw_calls)
	{
		//if(!it.cam->inside_frustum(it.model->get_AABB())) continue;

		it.shader->use();

		it.shader->upload_projection(it.cam->get_projection());
		it.shader->upload_view(it.cam->get_view());
		
		it.shader->upload_model(it.model->transform.get_combined());
		
		assert(it.model->get_material() != nullptr);
		assert(it.model->get_material()->texture != nullptr);
		it.model->get_material()->texture->bind();

		it.model->draw();

		t_c += it.model->get_vertex_array()->get_triangle_count();
		v_c += it.model->get_vertex_array()->get_vertex_count();
	}

	ImGui::Begin("RenderPass");
	ImGui::Text("Triangle count: %i",t_c);
	ImGui::Text("Vertex count: %i", v_c);
	ImGui::End();

	draw_calls.clear(); 
}