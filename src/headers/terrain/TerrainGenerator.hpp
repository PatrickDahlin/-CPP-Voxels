#ifndef _TERRAINGENERATOR_HPP_
#define _TERRAINGENERATOR_HPP_

#include "core/Common.hpp"
#include "graphics/Model.hpp"
#include "voxel/VoxelData.hpp"
#include <algorithm>

#include "core/PerlinNoise.hpp"
#include "voxel/MarchingCubes.hpp"

static MarchingCubes m_cubes;

void generate_voxels(vec3 pos, VoxelData* data)
{
	assert(data);

	for(int i = 0; i < data->get_width(); i++)
	{
		for(int j = 0; j < data->get_height(); j++)
		{
			for(int k = 0; k < data->get_width(); k++)
			{
				float x = i + pos.x;
				float y = j + pos.y;
				float z = k + pos.z;
				float tmp = fperlin_fractal(x * 0.23f,y * 0.13f,z * 0.23f, 2, 0.5f);
				tmp = std::min(1.0f,std::max(-1.0f,tmp));
				tmp = (tmp + 1.0f) / 2.0f;
				data->set_value_at_index(i,j,k, (unsigned char)(tmp*255));
				
			}
		}
	}

}

Model* generate_terrain(VoxelData& data)
{
	MCMesh* mesh = m_cubes.Evaluate(&data, 128);

	Model* voxel_model = new Model();
	assert(voxel_model);
	voxel_model->set_vertices(mesh->vertices);
	voxel_model->set_texcoords(mesh->texcoords);
	voxel_model->set_normals(mesh->normals);	
	return voxel_model;
}

#endif