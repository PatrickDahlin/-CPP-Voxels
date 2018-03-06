#include "terrain/Terrain.hpp"
#include "core/Transform.hpp"

#include "graphics/ShaderProgram.hpp"
#include "graphics/GLTexture.hpp"
#include "core/Files.hpp"

#include <algorithm>

Terrain::Terrain() :
center(0,0,0),
chunks(),
draw_dist(0)
{}

Terrain::~Terrain()
{}

void Terrain::set_center(vec3 pos)
{
	center = pos;
	remove_outliers();
	fill_empty_slots();
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
				ivec3 ipos = ivec3(i,j,k);
				ivec3 pos = vec3(i*CHUNK_SIZE,j*CHUNK_SIZE,k*CHUNK_SIZE);
				TerrainChunk c = TerrainChunk(pos, CHUNK_SIZE_PLUSONE, terrain_atlas, terrain_shader);
				c.init();
				chunks.emplace_back(c);
				chunk_lookup[ipos] = &c;
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
	
	chunks.clear();
}

void Terrain::fill_empty_slots()
{
	int side_len = draw_dist*2 + 1;
	vec3 center_chunk_offset(CHUNK_SIZE / 2.0f, CHUNK_SIZE / 2.0f, CHUNK_SIZE / 2.0f);

	for(int i=0; i < side_len; i++)
	{
		for(int j=0; j < side_len; j++)
		{
			for(int k=0; k < side_len; k++)
			{


				ivec3 ipos(i,j,k);
				vec3 cur_pos = vec3(i*CHUNK_SIZE,j*CHUNK_SIZE,k*CHUNK_SIZE) + center_chunk_offset;
				float dist = glm::length(center - cur_pos);

				if(dist > (float)(draw_dist*CHUNK_SIZE)) continue;

				auto it = chunk_lookup.find(ipos);
				if(it == chunk_lookup.end())
				{
					ivec3 pos = vec3(i*CHUNK_SIZE,j*CHUNK_SIZE,k*CHUNK_SIZE);
					TerrainChunk c = TerrainChunk(pos, CHUNK_SIZE_PLUSONE, terrain_atlas, terrain_shader);
					c.init();
					chunks.emplace_back(c);
					chunk_lookup[ipos] = &c;

					//printf("Added chunk %.2f %i,%i,%i status: ", dist, ipos.x, ipos.y, ipos.z);
					//if(chunk_lookup.find(ipos) == chunk_lookup.end()) printf("NOT ");
					//printf("OK\n");
				}
			}
		}
	}
}

void Terrain::remove_outliers()
{
	if(chunks.size() == 0) return;

	vec3 center_chunk_offset(CHUNK_SIZE / 2.0f, CHUNK_SIZE / 2.0f, CHUNK_SIZE / 2.0f);

	int n = 0;
	for(int i=chunks.size()-1;i>=0;i--)
	{
		vec3 c_pos = chunks[i].get_chunk_pos();
		vec3 cur_pos = c_pos + center_chunk_offset;
		ivec3 ipos((int)(c_pos.x/CHUNK_SIZE),
					(int)(c_pos.y/CHUNK_SIZE),
					(int)(c_pos.z/CHUNK_SIZE));

		float dist = glm::length(center - cur_pos); // vec3.length gives dimension not length of vector!!!!!!!!!!11!!1111!

		if(dist > (float)(draw_dist*CHUNK_SIZE))
		{
			TerrainChunk c = chunks[i];
			c.dispose();
			n++;

			auto it = chunk_lookup.find(ipos);
			if(it != chunk_lookup.end()) chunk_lookup.erase(it);
			//printf("Remove chunk %i,%i,%i status: ",ipos.x,ipos.y,ipos.z);

			//if(chunk_lookup.find(ipos) != chunk_lookup.end()) printf("NOT ");

			//printf(" OK\n");

			chunks.erase(chunks.begin() + i); // Erase is slow, prefer std::remove
		}
	}

}
