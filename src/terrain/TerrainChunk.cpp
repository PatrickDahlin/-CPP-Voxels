#include "terrain/TerrainChunk.hpp"
#include "graphics/RenderPass.hpp"
#include "graphics/GLTexture.hpp"
#include "graphics/Model.hpp"
#include "graphics/ShaderProgram.hpp"
#include "graphics/Material.hpp"
#include "graphics/RenderPass.hpp"
#include "core/Common.hpp"

#include "terrain/TerrainGenerator.hpp"

TerrainChunk::TerrainChunk(vec3 pos, int size, GLTexture* tex, ShaderProgram* shader) :
size(size),
data(nullptr),
mesh(nullptr),
chunk_pos(pos),
mesh_tex(tex),
shader(shader)
{}

TerrainChunk::~TerrainChunk()
{}

void TerrainChunk::init()
{
	assert(!data);

	data = new VoxelData(size, size);
	data->init();
	generate_voxels(chunk_pos, data);

	mesh = generate_terrain_alloc(*data);
	mesh->transform.set_position(chunk_pos);
	Material* m = new Material();
	m->texture = mesh_tex;
	assert(!mesh->get_material());
	mesh->set_material(m);
	assert(mesh);
}

void TerrainChunk::render(RenderPass& pass)
{
	assert(shader && mesh);
	pass.draw_model(mesh, shader);
}

void TerrainChunk::update(float delta)
{

}

void TerrainChunk::dispose()
{
	if(mesh) {
		delete mesh->get_material();
		mesh->dispose();
		delete mesh;
		mesh = nullptr;
	}
	if(data){
		data->dispose();
		delete data;
		data = nullptr;
	} 
}


void TerrainChunk::set_active(bool active)
{
	this->active = active;
}

bool TerrainChunk::is_active() const
{
	return active;
}

vec3 TerrainChunk::get_chunk_pos() const
{
	return chunk_pos;
}


int TerrainChunk::get_chunk_size() const
{
	return size;
}