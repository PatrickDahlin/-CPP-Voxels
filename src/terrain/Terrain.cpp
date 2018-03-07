	#include "terrain/Terrain.hpp"
	#include "core/Transform.hpp"
	#include <iostream>

	#include "graphics/ShaderProgram.hpp"
	#include "graphics/GLTexture.hpp"
	#include "core/Files.hpp"

	#include <algorithm>

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
		vec3 new_origin = pos / 16.0f;
		new_origin.x = floor(new_origin.x) - (float)draw_dist;
		new_origin.y = floor(new_origin.y) - (float)draw_dist;
		new_origin.z = floor(new_origin.z) - (float)draw_dist;

		if(terrain_origin != new_origin)
			printf("Center in new chunk: %.1f,%.1f,%.1f\n",new_origin.x,new_origin.y,new_origin.z);
		else 
			return;
		terrain_origin = new_origin;

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
		assert(drawdist > 0);
		draw_dist = drawdist;
		remove_outliers();
		fill_empty_slots();
	}

	int Terrain::get_draw_dist() const
	{
		return draw_dist;
	}

	void Terrain::init()
	{
		printf("init terrain\n");
		
		dispose();

		std::string v_src = read_file("data/shaders/Voxel-vert.glsl");
		std::string f_src = read_file("data/shaders/Voxel-frag.glsl");
		std::string head = read_file("data/shaders/Shader_Header.glsl");
		terrain_shader = new ShaderProgram(v_src.c_str(), f_src.c_str(), head.c_str());

		terrain_atlas = load_image("data/textures/grass.jpg",ColorFormat::RGB);

		if(!draw_dist) return;

		chunk_drawdist_update();
	}

	void Terrain::chunk_drawdist_update()
	{
		return;
		int side_len = 2 * draw_dist + 1;
		for(int i = 0; i < side_len; i++)
		{
			for(int j = 0; j < side_len; j++)
			{
				for(int k = 0; k < side_len; k++)
				{
					//ivec3 ipos = ivec3(i,j,k);
					ivec3 pos = vec3(i*CHUNK_SIZE,j*CHUNK_SIZE,k*CHUNK_SIZE);
					TerrainChunk* c = new TerrainChunk(pos, CHUNK_SIZE_PLUSONE, terrain_atlas, terrain_shader);
					c->init();
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
		chunks.clear();
	}

	void Terrain::fill_empty_slots()
	{
		int side_len = draw_dist*2 + 1;
		vec3 center_chunk_offset(CHUNK_SIZE / 2.0f, CHUNK_SIZE / 2.0f, CHUNK_SIZE / 2.0f);

		/*if(chunk_lookup.size() > 0){
			printf("currently in the list:\n");
			ivec3 tmp2(1,1,1);
			bool found = false;
			for(auto& it : chunk_lookup)
			{
				//printf("(%i,%i,%i) ",it.x,it.y,it.z);
				if(it == tmp2) found = true;	
			}
			printf("\n");


			printf("first value was ");
			if(!found) printf("NOT ");
			printf("found\n");
		}*/
		for(int i=0; i < side_len; i++)
		{
			for(int j=0; j < side_len; j++)
			{
				for(int k=0; k < side_len; k++)
				{
					// Local chunk coords within terrain
					ivec3 ipos(i,j,k);
					
					// World pos for current chunk origin
					//vec3 chunk_origin = vec3((float)i*CHUNK_SIZE,(float)j*CHUNK_SIZE,(float)k*CHUNK_SIZE);
					//chunk_origin += terrain_origin * (float)CHUNK_SIZE;
					
					vec3 chunk_origin((float)i,(float)j,(float)k);
					chunk_origin += terrain_origin;
					chunk_origin *= (float)CHUNK_SIZE;

					// Calculate distance to center of chunk
					float dist = glm::length(center - (chunk_origin + center_chunk_offset));
					if(dist > (float)(draw_dist*CHUNK_SIZE)) continue;

					// Look for chunk if location is occupied				
					/*auto it = chunk_lookup.find(ipos);
					if(it != chunk_lookup.end()) 
						continue;
					//printf("currently in the list:\n");
					*/
					bool found = false;
					for(auto& it : chunk_lookup)
					{
						//printf("(%i,%i,%i) ",it.x,it.y,it.z);
						if(it == ipos)
						{
							//printf("Found!!! %i, %i, %i == %i, %i, %i value = %i\n", it.first.x, it.first.y, it.first.z, ipos.x, ipos.y, ipos.z, it.second);
							found = true;
							break;
						}
					}
					//printf("\n");

					if(found) 
						continue;
					
					
					
					//auto it = std::find(chunk_lookup.begin(), chunk_lookup.end(), ipos);
					//if(it == chunk_lookup.end())
					{

						TerrainChunk* c = new TerrainChunk(chunk_origin, CHUNK_SIZE_PLUSONE, terrain_atlas, terrain_shader);
						c->init();
						chunks.emplace_back(c);
						//chunk_lookup[ipos] = chunks.size()-1;
						chunk_lookup.emplace_back(ipos);
						printf("Added chunk at (%i,%i,%i)\n",ipos.x,ipos.y,ipos.z);

					}
				}
			}
		}
	}

	void Terrain::remove_outliers()
	{
		//if(chunks.size() == 0) return;

		vec3 center_chunk_offset(CHUNK_SIZE / 2.0f, CHUNK_SIZE / 2.0f, CHUNK_SIZE / 2.0f);

		int n = 0;

		for(auto it = chunks.begin(); it != chunks.end();)
		{
			
			vec3 chunk_origin = (*it)->get_chunk_pos();
			ivec3 ipos((int)chunk_origin.x/16,
						(int)chunk_origin.y/16,
						(int)chunk_origin.z/16);

			ipos.x -= (int)terrain_origin.x;
			ipos.y -= (int)terrain_origin.y;
			ipos.z -= (int)terrain_origin.z;

			float dist = glm::length(center - (chunk_origin + center_chunk_offset)); 
			
			if(dist <= (float)(draw_dist*CHUNK_SIZE)) 
			{
				it++;
				continue;
			}

			(*it)->dispose();
			delete (*it);

			printf("Trying to remove (%i,%i,%i) from lookup\n",ipos.x,ipos.y,ipos.z);

			for(auto it2 = chunk_lookup.begin(); it2 != chunk_lookup.end();)
			{
				//printf("comparing against (%i,%i,%i)\n",(*it2).x,(*it2).y,(*it2).z);
				if( (*it2).x == ipos.x &&
					(*it2).y == ipos.y &&
					(*it2).z == ipos.z )
				{
					printf("found and removed\n");
					//wprintf("Removed lookup %i,%i,%i\n",(*it2).x, (*it2).y, (*it2).z);
					it2 = chunk_lookup.erase(it2);
				}
				else
					it2++;
			}//*/

			/*auto it2 = chunk_lookup.find(ipos);
			while(it2 != chunk_lookup.end())
			{
				//printf("removed a value %i %i %i\n", ipos.x, ipos.y, ipos.z);
				it2 = chunk_lookup.erase(it2);
				it2 = std::find_if(it2, chunk_lookup.end(), 
				[ipos](const std::pair<ivec3, int>& m) -> bool { return m.first == ipos; });
			}///*/

			it = chunks.erase(it);
		}

	}
