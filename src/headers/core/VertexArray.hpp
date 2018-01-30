#ifndef _VERTEXARRAY_HPP_
#define _VERTEXARRAY_HPP_
// 28.01.2018 @ Patrick Dahlin

#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/vec2.hpp>
#include "Shader_vars.hpp"

using namespace glm;
using namespace std;


class GLBuffer;

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void clear();

	void set_vertices(vector<vec3> verts);
	void set_normals(vector<vec3> norms);
	void set_colors(vector<vec4> cols);
	void set_texcoords(vector<vec2> coords);

	void add_vertex(vec3 vert);
	void add_vertex(float x, float y, float z);
	void add_normal(vec3 norm);
	void add_normal(float x, float y, float z);
	void add_color(vec4 color);
	void add_color(float r, float g, float b, float a);
	void add_texcoord(vec2 coord);
	void add_texcoord(float u, float v);

	void upload_data();

	void draw();

	void bind();

	void unbind();

	void dispose();

private:
	
	vector<vec3>			vertices;
	vector<vec3>			normals;
	vector<vec4>			colors;
	vector<vec2>			texcoords;

	unsigned int 			vao;
	vector<GLBuffer>		glbuffers;

};

#endif