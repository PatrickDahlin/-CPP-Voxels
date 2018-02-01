#include "../headers/core/Model.hpp"
#include "../headers/core/VertexArray.hpp"
#include "../headers/core/Material.hpp"

Model::Model() :
needs_upload(false)
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

	vert_arr->draw();
	needs_upload = false;
}