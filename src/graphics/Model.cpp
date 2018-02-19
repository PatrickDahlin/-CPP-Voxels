#include "graphics/Model.hpp"
#include "graphics/VertexArray.hpp"
#include "graphics/Material.hpp"

#include <stdio.h>

#include "graphics/GLBuffer.hpp"
#include <tiny_obj_loader.h>

Model::Model() :
needs_upload(false),
material(nullptr)
{
	vert_arr = new VertexArray();
}

Model::~Model()
{

}

void Model::dispose()
{
	vert_arr->dispose();
	delete vert_arr;
}

VertexArray* Model::get_vertex_array() const
{
	return vert_arr;
}

void Model::set_material(Material* mat)
{
	material = mat;
}

Material* Model::get_material() const
{
	return material;
}

void Model::set_vertices(vector<vec3> verts)
{
	vert_arr->set_vertices(verts);
	needs_upload = true;
}

void Model::set_normals(vector<vec3> norms)
{
	vert_arr->set_normals(norms);
	needs_upload = true;
}

void Model::set_colors(vector<vec4> cols)
{
	vert_arr->set_colors(cols);
	needs_upload = true;
}

void Model::set_texcoords(vector<vec2> coords)
{
	vert_arr->set_texcoords(coords);
	needs_upload = true;
}

void Model::draw()
{
	if(needs_upload)
		vert_arr->upload_data();

	vert_arr->draw(RenderMode::TRIANGLE);
	
	needs_upload = false;
}



Model* load_obj_from_file(std::string path)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	
	std::string err;
	bool ret = tinyobj::LoadObj(&attrib, 
								&shapes, 
								&materials, 
								&err, 
								path.c_str(),
								path.substr(0,path.find_last_of("/")+1).c_str());

	if (!err.empty()) { // `err` may contain warning message.
		printf("Error loading obj file!\n%s\n",err.c_str());
		return nullptr;
	}

	Model* m = new Model();
	m->set_material(new Material());
	//printf("Loading texture for model: %s\n",materials[0].diffuse_texname.c_str());
	m->get_material()->texture = new GLTexture(ColorFormat::RGB, nullptr, 0,0);//load_image(materials[0].diffuse_texname.c_str());

	std::vector<vec3> verts;
	std::vector<vec3> norms;
	std::vector<vec2> texcoords;
	std::vector<vec4> colors;

	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				tinyobj::real_t vx = attrib.vertices[3*idx.vertex_index+0];
				tinyobj::real_t vy = attrib.vertices[3*idx.vertex_index+1];
				tinyobj::real_t vz = attrib.vertices[3*idx.vertex_index+2];
				tinyobj::real_t nx = attrib.normals[3*idx.normal_index+0];
				tinyobj::real_t ny = attrib.normals[3*idx.normal_index+1];
				tinyobj::real_t nz = attrib.normals[3*idx.normal_index+2];
				tinyobj::real_t tx = attrib.texcoords[2*idx.texcoord_index+0];
				tinyobj::real_t ty = attrib.texcoords[2*idx.texcoord_index+1];
				tinyobj::real_t red = attrib.colors[3*idx.vertex_index+0];
      			tinyobj::real_t green = attrib.colors[3*idx.vertex_index+1];
      			tinyobj::real_t blue = attrib.colors[3*idx.vertex_index+2];


				// @TODO @Performance This is NOT performance friendly, just upload the data
				// straight from the buffers we already have, instead of copying it
				verts.emplace_back(vec3(vx,vy,vz));
				norms.emplace_back(vec3(nx,ny,nz));
				texcoords.emplace_back(vec2(tx,ty));
				colors.emplace_back(vec4(red,green,blue,1.0f));
			}
			index_offset += fv;

			// per-face material
			shapes[s].mesh.material_ids[f];
		}
	}

	m->set_vertices(verts);
	m->set_normals(norms);
	m->set_texcoords(texcoords);
	m->set_colors(colors);

	return m;
}