#ifndef _TERRAINGENERATOR_HPP_
#define _TERRAINGENERATOR_HPP_

#include "core/Common.hpp"
#include "graphics/Model.hpp"
#include "voxel/VoxelData.hpp"

#include "voxel/MarchingCubes.hpp"

static MarchingCubes m_cubes;

Model* generate_terrain(vec3 position, VoxelData& data)
{
	MCMesh* mesh = m_cubes.Evaluate(&data, 128);

	Model* voxel_model = new Model();
	voxel_model->set_vertices(mesh->vertices);
	voxel_model->set_texcoords(mesh->texcoords);
	voxel_model->set_normals(mesh->normals);	
	return voxel_model;
}

#endif