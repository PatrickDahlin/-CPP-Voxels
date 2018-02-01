#ifndef _MODEL_HPP_
#define _MODEL_HPP_

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include <vector>
#include "core/Transform.hpp"

using namespace glm;
using namespace std;
class VertexArray;
class Material;

class Model {
public:
    Model();
    ~Model();

	void set_vertices(vector<vec3> verts);
	void set_normals(vector<vec3> norms);
	void set_colors(vector<vec4> cols);
	void set_texcoords(vector<vec2> coords);

	void set_material(Material* mat);

	void draw();

	void dispose();

	Material* get_material() const;

	Transform		transform;
	
protected:

	bool			needs_upload;
    
    VertexArray*	vert_arr;
	Material*		material;

};

#endif