#ifndef _TERRAINCHUNK_HPP_
#define _TERRAINCHUNK_HPP_

#include "core/Common.hpp"
#include "voxel/VoxelData.hpp"
#include "core/Disposable.hpp"
#include "core/Transform.hpp"
#include "graphics/Model.hpp"
#include "graphics/RenderPass.hpp"

// Needs to be size +1 since we need one voxel that overlaps on each chunk border
constexpr int CHUNK_SIZE_PLUSONE = 33;
constexpr int CHUNK_SIZE = 32;

class GLTexture;
class ShaderProgram;

class TerrainChunk : public Disposable {
public:
	TerrainChunk(vec3 pos, int size, GLTexture* tex, ShaderProgram* shader);
	virtual ~TerrainChunk();
	//T(T const& other) = delete;
	TerrainChunk(TerrainChunk const& other) = delete;

	void set_active(bool active);

	bool is_active() const;

	void set_position(vec3 pos);

	void init();

	void render(RenderPass& pass);
	
	void update(float delta);
	
	void dispose();

	vec3 get_chunk_pos() const;

	int get_chunk_size() const;

private:

	bool			active;
	int				size;
	VoxelData*		data;

	Model*			mesh;
	vec3			chunk_pos;

	GLTexture*		mesh_tex;
	ShaderProgram*	shader;

};

#endif