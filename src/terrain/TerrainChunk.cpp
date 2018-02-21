#include "terrain/TerrainChunk.hpp"
#include "graphics/RenderPass.hpp"
#include "graphics/GLTexture.hpp"
#include "graphics/Model.hpp"
#include "graphics/ShaderProgram.hpp"
#include "core/Common.hpp"

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
	
}

void TerrainChunk::update(float delta)
{

}

void TerrainChunk::dispose()
{

}


void TerrainChunk::set_active(bool active)
{
	this->active = active;
}

bool TerrainChunk::is_active() const
{
	return active;
}
