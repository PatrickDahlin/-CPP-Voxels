#include "terrain/Terrain.hpp"
#include "core/Transform.hpp"
#include <iostream>

#include "graphics/ShaderProgram.hpp"
#include "graphics/GLTexture.hpp"
#include "core/Files.hpp"

#include <algorithm>
#include <imgui/imgui.h>

Terrain::Terrain() :
center(0,0,0),
terrain_origin(0,0,0),
chunk_lookup(),
chunks(),
draw_dist(0)
{}

Terrain::~Terrain()
{}

void Terrain::set_center(vec3 pos)
{
	vec3 new_origin = pos / (float)CHUNK_WORLD_SIZE;
	new_origin.x = floor(new_origin.x) - (float)draw_dist;
	new_origin.y = floor(new_origin.y) - (float)draw_dist;
	new_origin.z = floor(new_origin.z) - (float)draw_dist;

	//if(terrain_origin != new_origin)
	//	printf("Center in new chunk: %.1f,%.1f,%.1f\n",new_origin.x,new_origin.y,new_origin.z);
	//else 
	//	return;

	terrain_origin = new_origin;

	center = pos;
	//remove_outliers();
	//fill_empty_slots();
}

vec3 Terrain::get_center() const
{
	return center;
}

void Terrain::set_draw_dist(int drawdist)
{
	assert(drawdist > 0);
	draw_dist = drawdist;
}

int Terrain::get_draw_dist() const
{
	return draw_dist;
}

void Terrain::init()
{
	printf("init terrain\n");
	
	std::string v_src = read_file("data/shaders/Voxel-vert.glsl");
	std::string f_src = read_file("data/shaders/Voxel-frag.glsl");
	std::string head = read_file("data/shaders/Shader_Header.glsl");
	terrain_shader = new ShaderProgram(v_src.c_str(), f_src.c_str(), head.c_str());

	terrain_atlas = load_image("data/textures/grass.jpg",ColorFormat::RGB);

	assert(terrain_shader);
	assert(terrain_atlas);

	printf("Done init\n");
}

void Terrain::update(float delta)
{
	// Find nearest unpopulated chunkpos
	// IF found, put a new/inactive chunk there
	// generate mesh for it
	ImGui::Begin("Terrain");
	ImGui::Text("Chunk lookup map size: %i",(int)chunk_lookup.size());
	ImGui::Text("Chunks: %i", (int)chunks.size());
	ImGui::End();

	remove_outliers();
	fill_empty_slots();
}

void Terrain::render(RenderPass& pass)
{
	// Chunk rendering is handled here
	
	for(auto& it : chunks)
	{
		it->render(pass);
	}

}

void Terrain::dispose()
{
	printf("terrain dispose\n");

	for(auto& it : chunks)
	{
		it->dispose();
		delete it;
	}
	for(auto& it : inactive_chunks)
	{
		it->dispose();
		delete it;
	}
	chunks.clear();
	inactive_chunks.clear();

	terrain_shader->dispose();
	delete terrain_shader;
}

void Terrain::fill_empty_slots()
{
	int side_len = draw_dist*2 + 1;
	vec3 center_chunk_offset(CHUNK_WORLD_SIZE / 2.0f, CHUNK_WORLD_SIZE / 2.0f, CHUNK_WORLD_SIZE / 2.0f);

	int n = 0;

	for(int i=0; i < side_len; i++)
	{
		for(int j=0; j < side_len; j++)
		{
			for(int k=0; k < side_len; k++)
			{
				// Local chunk coords within terrain
				ivec3 ipos(i,j,k);
				
				ipos.x += terrain_origin.x;
				ipos.y += terrain_origin.y;
				ipos.z += terrain_origin.z;

				// World pos for current chunk origin
				vec3 chunk_origin((float)i,(float)j,(float)k);
				chunk_origin += terrain_origin;
				chunk_origin *= (float)CHUNK_WORLD_SIZE;

				// Calculate distance to center of chunk
				float dist = glm::length(center - (chunk_origin + center_chunk_offset));
				if(dist > (float)(draw_dist*CHUNK_WORLD_SIZE)) continue;

				// Look for chunk if location is occupied
				bool found = false;
				for(auto& it : chunk_lookup)
				{
					//printf("(%i,%i,%i) ",it.x,it.y,it.z);
					if(it.first == ipos)
					{
						//printf("Found!!! %i, %i, %i == %i, %i, %i value = %i\n", it.first.x, it.first.y, it.first.z, ipos.x, ipos.y, ipos.z, it.second);
						found = true;
						break;
					}
				}

				if(found) 
					continue;

				TerrainChunk* chunk = nullptr;
				if(inactive_chunks.size() > 0)
				{
					// Look for inactive chunk to put here
					for(auto it = inactive_chunks.begin(); it != inactive_chunks.end();)
					{
						if( (*it) != nullptr )
						{
							chunk = *it;
							inactive_chunks.erase(it);
							break;
						}
						it++;
					}
					
				}
				
				if(!chunk)
					chunk = new TerrainChunk(chunk_origin, CHUNK_SIZE_PLUSONE, terrain_atlas, terrain_shader);
				

				//TerrainChunk* c = new TerrainChunk(chunk_origin, CHUNK_SIZE_PLUSONE, terrain_atlas, terrain_shader);
				chunk->set_active(true);
				chunk->set_position(chunk_origin);
				chunk->init();
				chunks.emplace_back(chunk);
				chunk_lookup[ipos] = chunk;
				n++;
			}
		}
	}
}



void Terrain::remove_outliers()
{

	vec3 center_chunk_offset(CHUNK_WORLD_SIZE / 2.0f, CHUNK_WORLD_SIZE / 2.0f, CHUNK_WORLD_SIZE / 2.0f);

	for(auto it = chunks.begin(); it != chunks.end();)
	{
		
		vec3 chunk_origin = (*it)->get_chunk_pos();
		ivec3 ipos((int)chunk_origin.x/CHUNK_WORLD_SIZE,
					(int)chunk_origin.y/CHUNK_WORLD_SIZE,
					(int)chunk_origin.z/CHUNK_WORLD_SIZE);


		float dist = glm::length(center - (chunk_origin + center_chunk_offset)); 
		
		if(dist <= (float)(draw_dist*CHUNK_WORLD_SIZE)) 
		{
			it++;
			continue;
		}

		//printf("Trying to remove (%i,%i,%i) from lookup\n",ipos.x,ipos.y,ipos.z);

		for(auto it2 = chunk_lookup.begin(); it2 != chunk_lookup.end();)
		{
			//printf("comparing against (%i,%i,%i)\n",(*it2).x,(*it2).y,(*it2).z);
			if( (*it2).first.x == ipos.x &&
				(*it2).first.y == ipos.y &&
				(*it2).first.z == ipos.z )
			{
				//wprintf("Removed lookup %i,%i,%i\n",(*it2).x, (*it2).y, (*it2).z);
				it2 = chunk_lookup.erase(it2);
			}
			else
				it2++;
		}

		(*it)->set_active(false);
		inactive_chunks.emplace_back(*it);

		it = chunks.erase(it);
	}

}
