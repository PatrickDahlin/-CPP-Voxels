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

struct my_ivec3 {
	my_ivec3(int x, int y, int z) : x(x), y(y), z(z) {}
	int x, y, z;
};

inline bool operator==(const my_ivec3& left, const my_ivec3& right)
{
	return left.x == right.x && left.y == right.y && left.z == right.z;
}

namespace std {
	template<> struct hash<my_ivec3>
	{
		typedef my_ivec3 arg_type;
		typedef std::size_t result;
		result operator()(arg_type const& arg) const noexcept
		{
			std::size_t h1 = std::hash<int>{}(arg.x);
			std::size_t h2 = std::hash<int>{}(arg.y);
			std::size_t h3 = std::hash<int>{}(arg.z);
			return h1 ^ (h2 >> 1) ^ (h3 >> 2);
		}
	};
}


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
	

	std::unordered_set<my_ivec3> 					chunk_lookup;
	std::vector<TerrainChunk*>					chunks;
	
	int 						draw_dist;

};

#endif