#include "terrain/TerrainChunk.hpp"
#include "graphics/RenderPass.hpp"
#include "graphics/GLTexture.hpp"
#include "graphics/Model.hpp"
#include "graphics/ShaderProgram.hpp"
#include "graphics/Material.hpp"
#include "graphics/RenderPass.hpp"
#include "core/Common.hpp"
#include "graphics/VertexArray.hpp"

#include "terrain/TerrainGenerator.hpp"

TerrainChunk::TerrainChunk(vec3 pos, int size, GLTexture* tex, ShaderProgram* shader) :
size(size),
data(nullptr),
mesh(nullptr),
chunk_pos(pos),
mesh_tex(tex),
shader(shader)
{
	assert(tex);
	assert(shader);
	assert(size > 0);
}

TerrainChunk::~TerrainChunk()
{}

void TerrainChunk::init()
{
	//printf("chunk init\n");
	if(!data) data = new VoxelData(size, size);
	data->init();
	generate_voxels(chunk_pos, data);

	if(mesh) 
	{
		assert(mesh->get_vertex_array());
		mesh->get_vertex_array()->clear();
	}
	else
	{
		mesh = new Model();
		Material* m = new Material();
		m->texture = mesh_tex;
		mesh->set_material(m);
	}
	
	assert(mesh);
	generate_terrain(mesh, *data);
	mesh->transform.set_position(chunk_pos);
	//printf("done chunk init\n");
}

void TerrainChunk::render(RenderPass& pass)
{
	if(!active) return;
	assert(shader);
	assert(mesh);
	pass.draw_model(mesh, shader);
}

void TerrainChunk::update(float delta)
{
	if(!active) return;
}

void TerrainChunk::dispose()
{
	printf("chunk dispose\n");
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

void TerrainChunk::set_position(vec3 pos)
{
	chunk_pos = pos;
}

vec3 TerrainChunk::get_chunk_pos() const
{
	return chunk_pos;
}


int TerrainChunk::get_chunk_size() const
{
	return size;
}