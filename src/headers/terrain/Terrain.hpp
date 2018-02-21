#ifndef _TERRAIN_HPP_
#define _TERRAIN_HPP_

#include "core/Common.hpp"
#include "core/Disposable.hpp"
#include "terrain/TerrainChunk.hpp"

#include <vector>
#include <glm/vec3.hpp>

class RenderPass;
typedef glm::vec3 vec3;

class Terrain : public Disposable {
public:
	Terrain();
	~Terrain();

	void set_center(vec3 pos);

	void set_draw_dist(int new_dist);

	void update(float delta);

	void render(RenderPass* pass);

	void dispose() override;

	vec3 get_center() const;

	int get_draw_dist() const;

private:

	void remove_outliers();

	vec3						center;
	std::vector<TerrainChunk>	chunks;
	std::vector<TerrainChunk>	inactive_chunks;
	int 						draw_dist;

};

#endif