#include "graphics/Primitives.hpp"

#include <vector>
#include <glm/glm.hpp>

typedef glm::vec3 v3;
typedef glm::vec2 v2;
typedef glm::vec4 v4;

/*
	p4 -- p3
	|     |
	p1 -- p2

*/
void add_face(std::vector<v3>& v,
				std::vector<v3>& n,
				std::vector<v4>& c,
				std::vector<v2>& t,
				v3 p1,
				v3 p2,
				v3 p3,
				v3 p4,
				v3 n1,
				v4 c1)
{
	v.emplace_back(p1);
	v.emplace_back(p2);
	v.emplace_back(p3);

	v.emplace_back(p1);
	v.emplace_back(p3);
	v.emplace_back(p4);

	t.emplace_back(v2(0,0));
	t.emplace_back(v2(1,0));
	t.emplace_back(v2(1,1));
	
	t.emplace_back(v2(0,0));
	t.emplace_back(v2(1,1));
	t.emplace_back(v2(0,1));

	n.emplace_back(n1);
	n.emplace_back(n1);
	n.emplace_back(n1);
	
	n.emplace_back(n1);
	n.emplace_back(n1);
	n.emplace_back(n1);

	c.emplace_back(c1);
	c.emplace_back(c1);
	c.emplace_back(c1);
	
	c.emplace_back(c1);
	c.emplace_back(c1);
	c.emplace_back(c1);
}

Model* Primitives::create_cube()
{
    Model* model = new Model();

	std::vector<v3> pos;
	std::vector<v3> norm;
	std::vector<v4> cols;
	std::vector<v2> tex;

	const float n = -0.5f;
	const float p = 0.5f;

	// FRONT
	add_face(pos, norm, cols, tex, 
			v3(n,n,n),v3(p,n,n),
			v3(p,p,n),v3(n,p,n),
			v3(0,0,-1),v4(1,1,1,1));

	// BACK
	add_face(pos, norm, cols, tex, 
			v3(p,n,p),v3(n,n,p),
			v3(n,p,p),v3(p,p,p),
			v3(0,0,1),v4(1,1,1,1));

	// TOP
	add_face(pos, norm, cols, tex, 
			v3(n,p,n),v3(p,p,n),
			v3(p,p,p),v3(n,p,p),
			v3(0,1,0),v4(1,1,1,1));

	// BOTTOM
	add_face(pos, norm, cols, tex, 
			v3(n,n,p),v3(p,n,p),
			v3(p,n,n),v3(n,n,n),
			v3(0,-1,0),v4(1,1,1,1));


	model->set_vertices(pos);
	model->set_normals(norm);
	model->set_colors(cols);
	model->set_texcoords(tex);	

    return model;
}


Model* Primitives::create_quad(const float w, const float h)
{
    Model* model = new Model();

    return model;
}

/*


void make_cube(Model& model, glm::vec4 col, glm::vec3 pos, float scale)
{
	float n = -0.5f;
	float p = 0.5f;

	std::vector<glm::vec3> verts;
	std::vector<glm::vec4> colors;
	std::vector<glm::vec2> texcoords;


	// FRONT
	verts.emplace_back(pos + glm::vec3(n,n,n) * scale);
	verts.emplace_back(pos + glm::vec3(p,n,n) * scale);
	verts.emplace_back(pos + glm::vec3(p,p,n) * scale);	
	verts.emplace_back(pos + glm::vec3(n,n,n) * scale);
	verts.emplace_back(pos + glm::vec3(p,p,n) * scale);
	verts.emplace_back(pos + glm::vec3(n,p,n) * scale);

	colors.emplace_back(col); colors.emplace_back(col); colors.emplace_back(col);
	colors.emplace_back(col); colors.emplace_back(col); colors.emplace_back(col);

	texcoords.emplace_back(0,0); texcoords.emplace_back(1,0); texcoords.emplace_back(1,1);
	texcoords.emplace_back(0,0); texcoords.emplace_back(1,1); texcoords.emplace_back(0,1);

	// TOP
	verts.emplace_back(pos + glm::vec3(n,p,n) * scale);
	verts.emplace_back(pos + glm::vec3(p,p,p) * scale);
	verts.emplace_back(pos + glm::vec3(p,p,n) * scale);
	verts.emplace_back(pos + glm::vec3(n,p,n) * scale);
	verts.emplace_back(pos + glm::vec3(p,p,p) * scale);
	verts.emplace_back(pos + glm::vec3(n,p,p) * scale);

	colors.emplace_back(col); colors.emplace_back(col); colors.emplace_back(col);
	colors.emplace_back(col); colors.emplace_back(col); colors.emplace_back(col);
	
	texcoords.emplace_back(0,0); texcoords.emplace_back(1,0); texcoords.emplace_back(1,1);
	texcoords.emplace_back(0,0); texcoords.emplace_back(1,1); texcoords.emplace_back(0,1);

	// LEFT
	verts.emplace_back(pos + glm::vec3(n,n,p) * scale);
	verts.emplace_back(pos + glm::vec3(n,p,n) * scale);
	verts.emplace_back(pos + glm::vec3(n,n,n) * scale);
	verts.emplace_back(pos + glm::vec3(n,n,p) * scale);
	verts.emplace_back(pos + glm::vec3(n,p,n) * scale);
	verts.emplace_back(pos + glm::vec3(n,p,p) * scale);

	colors.emplace_back(col); colors.emplace_back(col); colors.emplace_back(col);
	colors.emplace_back(col); colors.emplace_back(col); colors.emplace_back(col);

	texcoords.emplace_back(0,0); texcoords.emplace_back(1,0); texcoords.emplace_back(1,1);
	texcoords.emplace_back(0,0); texcoords.emplace_back(1,1); texcoords.emplace_back(0,1);

	// RIGHT
	verts.emplace_back(pos + glm::vec3(p,n,n) * scale);
	verts.emplace_back(pos + glm::vec3(p,n,p) * scale);
	verts.emplace_back(pos + glm::vec3(p,p,p) * scale);
	verts.emplace_back(pos + glm::vec3(p,n,n) * scale);
	verts.emplace_back(pos + glm::vec3(p,p,p) * scale);
	verts.emplace_back(pos + glm::vec3(p,p,n) * scale);
	
	colors.emplace_back(col); colors.emplace_back(col); colors.emplace_back(col);
	colors.emplace_back(col); colors.emplace_back(col); colors.emplace_back(col);
	
	texcoords.emplace_back(0,0); texcoords.emplace_back(1,0); texcoords.emplace_back(1,1);
	texcoords.emplace_back(0,0); texcoords.emplace_back(1,1); texcoords.emplace_back(0,1);

	// BOTTOM
	verts.emplace_back(pos + glm::vec3(n,n,p) * scale);
	verts.emplace_back(pos + glm::vec3(p,n,p) * scale);
	verts.emplace_back(pos + glm::vec3(p,n,n) * scale);
	verts.emplace_back(pos + glm::vec3(n,n,p) * scale);
	verts.emplace_back(pos + glm::vec3(p,n,n) * scale);
	verts.emplace_back(pos + glm::vec3(n,n,n) * scale);
	
	colors.emplace_back(col); colors.emplace_back(col); colors.emplace_back(col);
	colors.emplace_back(col); colors.emplace_back(col); colors.emplace_back(col);
	
	texcoords.emplace_back(0,0); texcoords.emplace_back(1,0); texcoords.emplace_back(1,1);
	texcoords.emplace_back(0,0); texcoords.emplace_back(1,1); texcoords.emplace_back(0,1);

	// BACK
	verts.emplace_back(pos + glm::vec3(n,n,p) * scale);
	verts.emplace_back(pos + glm::vec3(p,p,p) * scale);
	verts.emplace_back(pos + glm::vec3(p,n,p) * scale);
	verts.emplace_back(pos + glm::vec3(n,n,p) * scale);
	verts.emplace_back(pos + glm::vec3(n,p,p) * scale);
	verts.emplace_back(pos + glm::vec3(p,p,p) * scale);
	
	colors.emplace_back(col); colors.emplace_back(col); colors.emplace_back(col);
	colors.emplace_back(col); colors.emplace_back(col); colors.emplace_back(col);
	
	texcoords.emplace_back(0,0); texcoords.emplace_back(1,0); texcoords.emplace_back(1,1);
	texcoords.emplace_back(0,0); texcoords.emplace_back(1,1); texcoords.emplace_back(0,1);

	model.set_vertices(verts);
	model.set_colors(colors);
	model.set_texcoords(texcoords);
}

*/