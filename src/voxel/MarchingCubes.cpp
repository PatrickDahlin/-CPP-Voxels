#include "voxel/MarchingCubes.hpp"
#include "voxel/VoxelData.hpp"
#include "graphics/VertexArray.hpp"
#include <glm/glm.hpp>

MarchingCubes::MarchingCubes(){}
MarchingCubes::~MarchingCubes(){}

MCMesh* MarchingCubes::Evaluate(VoxelData* data, byte iso)
{
	int ntriang = 0;
	vec3 current;
	MCMesh* mesh = new MCMesh();
	// i is z
	for(int i = data->get_width()-2; i >= 0; i--)
	{
		// j is y
		for(int j = data->get_height()-2; j >= 0; j--)
		{
			// k is x
			for(int k = data->get_width()-2; k >= 0; k--)
			{
				current.x = i; current.y = j; current.z = k;

				#define d(x,y,z) data->get_value_at_index(x,y,z)

				unsigned char cubeindex = 0;
				if (data->get_value_at_index(i,j,k+1) < iso) 	cubeindex |= 1; // 0
				if (data->get_value_at_index(i+1,j,k+1) < iso) 	cubeindex |= 2; // 1
				if (data->get_value_at_index(i+1,j,k) < iso) 	cubeindex |= 4; // 2
				if (data->get_value_at_index(i,j,k) < iso) 		cubeindex |= 8; // 3
				if (data->get_value_at_index(i,j+1,k+1) < iso) 	cubeindex |= 16; //4
				if (data->get_value_at_index(i+1,j+1,k+1) < iso)cubeindex |= 32; //5
				if (data->get_value_at_index(i+1,j+1,k) < iso)	cubeindex |= 64; //6
				if (data->get_value_at_index(i,j+1,k) < iso) 	cubeindex |= 128;//7

				if(edgeTable[cubeindex] == 0)
					continue; // No geometry in this cube, either completely within or outside surface
				

				vec3 vertlist[12];

				if (edgeTable[cubeindex] & 1)
					vertlist[0] =
						interp(iso,p0,p1,d(i,j,k+1),d(i+1,j,k+1));
				if (edgeTable[cubeindex] & 2)
					vertlist[1] =
						interp(iso,p1,p2,d(i+1,j,k+1),d(i+1,j,k));
				if (edgeTable[cubeindex] & 4)
					vertlist[2] =
						interp(iso,p2,p3,d(i+1,j,k),d(i,j,k));
				if (edgeTable[cubeindex] & 8)
					vertlist[3] =
						interp(iso,p3,p0,d(i,j,k),d(i,j,k+1));
				if (edgeTable[cubeindex] & 16)
					vertlist[4] =
						interp(iso,p4,p5,d(i,j+1,k+1),d(i+1,j+1,k+1));
				if (edgeTable[cubeindex] & 32)
					vertlist[5] =
						interp(iso,p5,p6,d(i+1,j+1,k+1),d(i+1,j+1,k));
				if (edgeTable[cubeindex] & 64)
					vertlist[6] =
						interp(iso,p6,p7,d(i+1,j+1,k),d(i,j+1,k));
				if (edgeTable[cubeindex] & 128)
					vertlist[7] =
						interp(iso,p7,p4,d(i,j+1,k),d(i,j+1,k+1));
				if (edgeTable[cubeindex] & 256)
					vertlist[8] =
						interp(iso,p0,p4,d(i,j,k+1),d(i,j+1,k+1));
				if (edgeTable[cubeindex] & 512)
					vertlist[9] =
						interp(iso,p1,p5,d(i+1,j,k+1),d(i+1,j+1,k+1));
				if (edgeTable[cubeindex] & 1024)
					vertlist[10] =
						interp(iso,p2,p6,d(i+1,j,k),d(i+1,j+1,k));
				if (edgeTable[cubeindex] & 2048)
					vertlist[11] =
						interp(iso,p3,p7,d(i,j,k),d(i,j+1,k));

				for(int l = 0; triTable[cubeindex][l] != -1; l+= 3)
				{
					vec3 vert1 = vertlist[triTable[cubeindex][l]];
					vec3 vert2 = vertlist[triTable[cubeindex][l+1]];
					vec3 vert3 = vertlist[triTable[cubeindex][l+2]];
					
					mesh->vertices.emplace_back((vert1 + current));
					mesh->vertices.emplace_back((vert2 + current));
					mesh->vertices.emplace_back((vert3 + current));
					
					vec3 normal = glm::cross((vert2 - vert1),(vert3 - vert1));

					mesh->normals.emplace_back(normal);
					mesh->normals.emplace_back(normal);
					mesh->normals.emplace_back(normal);
					
					mesh->texcoords.emplace_back(vec2(0,0));
					mesh->texcoords.emplace_back(vec2(1,0));
					mesh->texcoords.emplace_back(vec2(1,1));

					ntriang++;
				}
			}
		}
	}
	mesh->tri_count = ntriang;
	return mesh;
}

vec3 MarchingCubes::interp(byte iso, vec3 p1, vec3 p2, byte val1, byte val2)
{
	if(abs(iso-val1) <= 0)
		return p1;
	if(abs(iso-val2) <= 0)
		return p2;
	if(abs(val1-val2) <= 0)
		return p1;

	double mu = ((double)iso - (double)val1) / ((double)val2 - (double)val1);

	return vec3(
		p1.x + mu * (p2.x - p1.x),
		p1.y + mu * (p2.y - p1.y),
		p1.z + mu * (p2.z - p1.z)
	);
}