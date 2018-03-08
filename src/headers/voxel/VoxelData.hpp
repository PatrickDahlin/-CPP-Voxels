#ifndef _VOXELDATA_HPP_
#define _VOXELDATA_HPP_

/*

Basic dataclass for voxel data, currently as
a flat array of bytes.

*/

class VoxelData {
public:
	VoxelData(int size);
	VoxelData(int width, int height);
	~VoxelData();
	VoxelData(VoxelData const& other) = delete;

	void init();

	unsigned char get_value_at_index(int x, int y, int z);

	void set_value_at_index(int x, int y, int z, unsigned char val);

	void dispose();

	int get_width() const;
	int get_height() const;

private:

	int get_index(int x, int y, int z);

	int				width, height;
	unsigned char*	data;
	int				data_length;

};

#endif