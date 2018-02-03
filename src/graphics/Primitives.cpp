#include "graphics/Primitives.hpp"

#include <vector>
#include <glm/glm.hpp>

typedef glm::vec3 v3;
typedef glm::vec2 v2;
typedef glm::vec4 v4;

Model* Primitives::create_cube()
{
    Model* model = new Model();

    
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