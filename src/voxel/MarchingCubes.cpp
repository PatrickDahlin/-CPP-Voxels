#include "voxel/MarchingCubes.hpp"
#include "voxel/VoxelData.hpp"
#include "graphics/VertexArray.hpp"

bool MarchingCubes::Evaluate(VoxelData* data, VertexArray* varr)
{
	v3 current;
	// i is z
	for(int i = data->get_width()-1; i >= 0; i--)
	{
		// j is y
		for(int j = data->get_height()-1; j >= 0; j--)
		{
			// k is x
			for(int k = data->get_width()-1; k >= 0; k--)
			{
				current.x = k; current.y = j; current.z = i;

				
			}
		}
	}

	return false;
}

v3 MarchingCubes::interp(byte iso, v3 p1, v3 p2, byte val1, byte val2)
{
	if(abs(iso-val1) <= 0)
		return p1;
	if(abs(iso-val2) <= 0)
		return p2;
	if(abs(val1-val2) <= 0)
		return p1;

	double mu = (iso - val1) / ((double)val2 - (double)val1);

	return v3(
		p1.x + mu * (p2.x - p1.x),
		p1.y + mu * (p2.y - p1.y),
		p1.z + mu * (p2.z - p1.z)
	);
}