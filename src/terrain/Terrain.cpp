#include "terrain/Terrain.hpp"
#include "core/Transform.hpp"

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
