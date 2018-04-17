#ifndef _AABB_HPP_
#define _AABB_HPP_

#include "core/Common.hpp"
#include <glm/vec3.hpp>

struct AABB {
	AABB() : min(0,0,0), max(0,0,0) {}
	vec3 min;
	vec3 max;
};

#endif