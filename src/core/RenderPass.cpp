#include "../headers/core/RenderPass.hpp"
#include "../headers/core/Macros.hpp"
#include "../headers/core/GLBuffer.hpp"
#include "../headers/core/ShaderProgram.hpp"
#include "../headers/core/Material.hpp"



RenderPass::RenderPass()
{

}

RenderPass::~RenderPass()
{

}

void RenderPass::draw_model(GLBuffer* buf, ShaderProgram* shader, Material* mat)
{
	DrawCall d;

	d.buf = buf;
	d.shader = shader;
	d.mat = mat;

	draw_calls.emplace_back(d);
}

void RenderPass::do_render()
{
	for(auto it : draw_calls)
	{
		it.shader->use();

		it.buf->bind();
		// This is just for testing, won't use buffers directly in the future
		it.buf->data_pointer(0, 3, GL_FLOAT, 3*sizeof(float), false, (void*)0);
		it.buf->draw();
		it.buf->unbind();
	}

	cout("There are ");
	cout(draw_calls.size());
	cout(" calls this frame\n");

	draw_calls.clear(); 
}