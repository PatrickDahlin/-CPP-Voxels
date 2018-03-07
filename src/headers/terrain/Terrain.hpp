#ifndef _TERRAIN_HPP_
#define _TERRAIN_HPP_

#include "core/Common.hpp"
#include "core/Disposable.hpp"
#include "terrain/TerrainChunk.hpp"
#include "graphics/RenderPass.hpp"

#include <vector>
#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_set>

class RenderPass;

class Terrain : public Disposable {
public:
	Terrain();
	~Terrain();

	void set_center(vec3 pos);

	void set_draw_dist(int new_dist);

	void init();

	void update(float delta);

	void render(RenderPass& pass);

	void dispose() override;

	vec3 get_center() const;

	int get_draw_dist() const;

private:

	void chunk_drawdist_update();

	void remove_outliers();

	void fill_empty_slots();

	ShaderProgram*				terrain_shader;
	GLTexture*					terrain_atlas;

	vec3						center;
	vec3						terrain_origin; // 0,0,0 origin of local chunk coords
	

	std::vector<ivec3> 					chunk_lookup;
	std::vector<TerrainChunk*>					chunks;
	
	int 						draw_dist;

};

#endif