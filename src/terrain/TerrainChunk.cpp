#include "terrain/TerrainChunk.hpp"
#include "graphics/RenderPass.hpp"
#include "graphics/GLTexture.hpp"
#include "graphics/Model.hpp"
#include "graphics/ShaderProgram.hpp"
#include "graphics/RenderPass.hpp"
#include "core/Common.hpp"

#include "terrain/TerrainGenerator.hpp"

TerrainChunk::TerrainChunk(int size, GLTexture* tex, ShaderProgram* shader) :
size(size),
data(size,size),
mesh_tex(tex),
shader(shader)
{}

TerrainChunk::~TerrainChunk()
{}

void TerrainChunk::init()
{
	for(int i=0; i < size; i++)
	{
		for(int j=0; j < size; j++)
		{
			for(int k=0; k < size; k++)
			{
				data.set_value_at_index(i,j,k,(i+j+k)%255);
			}
		}
	}
	mesh = generate_terrain(transform.get_position(), data);
}

void TerrainChunk::render(RenderPass& pass)
{
	pass.draw_model(mesh, shader, nullptr);
}

void TerrainChunk::update(float delta)
{

}

void TerrainChunk::dispose()
{
	if(mesh) delete mesh;
}


void TerrainChunk::set_active(bool active)
{
	this->active = active;
}

bool TerrainChunk::is_active() const
{
	return active;
}
