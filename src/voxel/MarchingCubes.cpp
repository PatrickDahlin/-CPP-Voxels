#include "voxel/MarchingCubes.hpp"
#include "voxel/VoxelData.hpp"
#include "graphics/VertexArray.hpp"

MarchingCubes::MarchingCubes(){}
MarchingCubes::~MarchingCubes(){}

int MarchingCubes::Evaluate(VoxelData* data, byte iso, std::vector<glm::vec3>* verts)
{
	int ntriang = 0;
	v3 current;
	// i is z
	for(int k = data->get_width()-2; k >= 0; k--)
	{
		// j is y
		for(int j = data->get_height()-2; j >= 0; j--)
		{
			// k is x
			for(int i = data->get_width()-2; i >= 0; i--)
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
				

				v3 vertlist[12];

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
					v3 vert1 = vertlist[triTable[cubeindex][l]];
					v3 vert2 = vertlist[triTable[cubeindex][l+1]];
					v3 vert3 = vertlist[triTable[cubeindex][l+2]];
					verts->emplace_back(vert1);
					verts->emplace_back(vert2);
					verts->emplace_back(vert3);
					ntriang++;
				}
			}
		}
	}

	return ntriang;
}

v3 MarchingCubes::interp(byte iso, v3 p1, v3 p2, byte val1, byte val2)
{
	if(abs(iso-val1) <= 0)
		return p1;
	if(abs(iso-val2) <= 0)
		return p2;
	if(abs(val1-val2) <= 0)
		return p1;

	double mu = ((double)iso - (double)val1) / ((double)val2 - (double)val1);

	return v3(
		p1.x + mu * (p2.x - p1.x),
		p1.y + mu * (p2.y - p1.y),
		p1.z + mu * (p2.z - p1.z)
	);
}