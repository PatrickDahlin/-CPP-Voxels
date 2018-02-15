#ifndef _MARCHINGCUBES_HPP_
#define _MARCHINGCUBES_HPP_

#include <glm/glm.hpp>

class VoxelData;
class VertexArray;

#define v3 glm::vec3
#define byte unsigned char

class MarchingCubes{
public:

	bool Evaluate(VoxelData* data, VertexArray* varr);

private:
	MarchingCubes();
	~MarchingCubes();

	v3 interp(byte iso, v3 p1, v3 p2, byte val1, byte val2);

};

#endif