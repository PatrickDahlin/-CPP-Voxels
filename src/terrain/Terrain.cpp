#include "terrain/Terrain.hpp"
#include "core/Transform.hpp"

#include "graphics/ShaderProgram.hpp"
#include "graphics/GLTexture.hpp"
#include "core/Files.hpp"

#include <algorithm>

Terrain::Terrain() :
center(0,0,0),
chunks(),
inactive_chunks(),
draw_dist(0)
{}

Terrain::~Terrain()
{}

void Terrain::set_center(vec3 pos)
{
	center = pos;
	remove_outliers();
}

vec3 Terrain::get_center() const
{
	return center;
}

void Terrain::set_draw_dist(int drawdist)
{
	draw_dist = drawdist;
	remove_outliers();
}

int Terrain::get_draw_dist() const
{
	return draw_dist;
}

void Terrain::init()
{
	printf("init terrain\n");
	if(!draw_dist) return;

	dispose();

	std::string v_src = read_file("data/shaders/Voxel-vert.glsl");
	std::string f_src = read_file("data/shaders/Voxel-frag.glsl");
	std::string head = read_file("data/shaders/Shader_Header.glsl");
	terrain_shader = new ShaderProgram(v_src.c_str(), f_src.c_str(), head.c_str());

	terrain_atlas = load_image("data/textures/grass.jpg",ColorFormat::RGB);

	chunk_drawdist_update();
}

void Terrain::chunk_drawdist_update()
{
	int side_len = 2 * draw_dist + 1;
	for(int i = 0; i < side_len; i++)
	{
		for(int j = 0; j < side_len; j++)
		{
			for(int k = 0; k < side_len; k++)
			{
				TerrainChunk c = TerrainChunk(vec3(i*16,j*16,k*16), 17, terrain_atlas, terrain_shader);
				c.init();
				chunks.emplace_back(c);
			}
		}
	}
}

void Terrain::update(float delta)
{
	// Find nearest unpopulated chunkpos
	// IF found, put a new/inactive chunk there
	// generate mesh for it
}

void Terrain::render(RenderPass& pass)
{
	// Chunk rendering is handled here
	
	for(auto& it : chunks)
	{
		it.render(pass);
	}

}

void Terrain::dispose()
{
	printf("terrain dispose\n");

	for(auto& it : chunks)
		it.dispose();
	for(auto& it : inactive_chunks)
		it.dispose();
	
	chunks.clear();
	inactive_chunks.clear();
}

void Terrain::remove_outliers()
{
	return;
	/*
	for(int i=chunks.size();i>=0;i--)
	{
		vec3 cur_pos = chunks[i].get_transform()->get_position();
		float32 dist = (center - cur_pos).length();
		if(dist > draw_dist)
		{
			TerrainChunk c = chunks[i];
			inactive_chunks.push_back(c);
			chunks.erase(chunks.begin() + i); // Erase is slow, prefer std::remove
		}
	}*/
}
