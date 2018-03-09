#ifndef _MODEL_HPP_
#define _MODEL_HPP_

#include "core/Common.hpp"
#include <vector>
#include "core/Transform.hpp"

#include <string>

using namespace glm;
using namespace std;
class VertexArray;
class Material;

struct AABB {
	AABB() : min(0,0,0), max(0,0,0) {}
	vec3 min;
	vec3 max;
};

class Model {
public:
    Model();
    virtual ~Model();

	void set_vertices(vector<vec3> verts);
	void set_normals(vector<vec3> norms);
	void set_colors(vector<vec4> cols);
	void set_texcoords(vector<vec2> coords);

	void set_material(Material* mat);

	void draw();

	virtual void dispose();

	Material* get_material() const;

	VertexArray* get_vertex_array() const;

	AABB get_AABB() const;
	
	void recalculate_AABB();

	Transform		transform;
	
protected:

	bool			needs_upload;
    
    VertexArray*	vert_arr;
	Material*		material;

	AABB			aabb;

};

extern Model* load_obj_from_file(std::string path);

#endif