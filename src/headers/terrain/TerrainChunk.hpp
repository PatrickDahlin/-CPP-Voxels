#ifndef _TERRAINCHUNK_HPP_
#define _TERRAINCHUNK_HPP_

#include "core/Common.hpp"
#include "voxel/VoxelData.hpp"
#include "core/Actor.hpp"

class GLTexture;
class ShaderProgram;

class TerrainChunk : public Actor {
public:
	TerrainChunk(int size, GLTexture* tex, ShaderProgram* shader);
	~TerrainChunk();

	void set_active(bool active);

	bool is_active() const;

	// Actor methods
	void init() override;
	void update(float delta) override;
	void dispose() override;

private:

	bool			active;
	int			size;
	VoxelData		data;

	GLTexture*		mesh_tex;
	ShaderProgram*	shader;

};

#endif