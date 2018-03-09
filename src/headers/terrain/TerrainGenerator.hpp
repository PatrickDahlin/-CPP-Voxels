#ifndef _TERRAINGENERATOR_HPP_
#define _TERRAINGENERATOR_HPP_

#include "core/Common.hpp"
#include "graphics/Model.hpp"
#include "voxel/VoxelData.hpp"
#include <algorithm>

#include "core/PerlinNoise.hpp"
#include "voxel/MarchingCubes.hpp"

static MarchingCubes m_cubes;

void generate_voxels(vec3 pos, float scale, VoxelData* data)
{
	assert(data);

	for(int i = 0; i < data->get_width(); i++)
	{
		for(int j = 0; j < data->get_height(); j++)
		{
			for(int k = 0; k < data->get_width(); k++)
			{
				float x = i*scale + pos.x;
				float y = j*scale + pos.y;
				float z = k*scale + pos.z;
				float tmp = fperlin_fractal(x * 0.014f,y * 0.014f,z * 0.014f, 2, 0.5f);
				tmp = std::min(1.0f,std::max(-1.0f,tmp));
				tmp = (tmp + 1.0f) / 2.0f;
				data->set_value_at_index(i,j,k, (unsigned char)(tmp*255));
				
			}
		}
	}

}

void generate_terrain(Model* model, VoxelData& data)
{
	m_cubes.Evaluate(model, &data, 128);
}

Model* generate_terrain_alloc(VoxelData& data)
{
	Model* voxel_model = new Model();

	m_cubes.Evaluate(voxel_model, &data, 128);
		
	return voxel_model;
}

#endif