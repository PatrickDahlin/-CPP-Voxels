#include "voxel/VoxelData.hpp"
#include "core/Common.hpp"

VoxelData::VoxelData(int size) :
width(size),
height(size),
data(nullptr),
data_length(0)
{}

VoxelData::VoxelData(int width, int height) :
width(width),
height(height),
data(nullptr),
data_length(0)
{}

VoxelData::~VoxelData()
{
}

void VoxelData::init()
{
	init_data();
}

int VoxelData::get_width() const
{
	return width;
}

int VoxelData::get_height() const
{
	return height;
}

void VoxelData::init_data() 
{
	if(data != nullptr)
		delete[] data;
	
	data_length = width * width * height;
	data = new unsigned char[data_length];	
}

unsigned char VoxelData::get_value_at_index(int x, int y, int z) 
{
	assert(x < width  && x >= 0);
	assert(z < width  && z >= 0);
	assert(y < height && y >= 0);
	return data[get_index(x,y,z)];
}

void VoxelData::set_value_at_index(int x, int y, int z, unsigned char val)
{
	assert(x < width  && x >= 0);
	assert(z < width  && z >= 0);
	assert(y < height && y >= 0);
	assert(data);
	data[get_index(x,y,z)] = val;
}

int VoxelData::get_index(int x, int y, int z) 
{
	return x + y * height + z * width * width;
}

void VoxelData::dispose()
{
	if(data != nullptr)
	{
		delete[] data;
		data = nullptr;
		data_length = 0;
	}
}
