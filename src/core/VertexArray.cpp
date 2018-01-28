#include "../headers/core/VertexArray.hpp"
#include "../headers/core/GLBuffer.hpp"
#include "../headers/core/Macros.hpp"
#include <GL\glew.h>


VertexArray::VertexArray() :
vertices(),
normals(),
colors(),
texcoords(),
vao(0),
glbuffers()
{

	glGenVertexArrays(1, &vao);
	
}

VertexArray::~VertexArray()
{

	glDeleteVertexArrays(1, &vao);

}

void VertexArray::upload_data()
{
	glBindVertexArray(vao);
	
	//
	//	Vertices
	//
	if(vertices.size() > 0)
	{
		GLBuffer verts = GLBuffer(vertices.data(), sizeof(vertices), vertices.size());
		verts.bind();
		verts.data_pointer(SHADER_POSITION_LOCATION, 3, GL_FLOAT, 3 * sizeof(float), false, BUFFER_OFFSET(0));
		glbuffers.emplace_back(verts);
	}
	else 
	{
		glBindVertexArray(0);
		return;	// We don't want to render if we have no vertices
	}
	//
	//	Normals
	//
	if(normals.size() > 0)
	{
		if(normals.size() != vertices.size())
		{
			error("VertexArray: Normal and Vertex count mismatch!");
			glBindVertexArray(0);
			return;
		}
		GLBuffer norms = GLBuffer(normals.data(), sizeof(normals), normals.size());
		norms.bind();
		norms.data_pointer(SHADER_NORMAL_LOCATION, 3, GL_FLOAT, 3 * sizeof(float), false, BUFFER_OFFSET(0));
		glbuffers.emplace_back(norms);
	}

	//
	//	Colors
	//
	if(colors.size() > 0)
	{
		if(colors.size() != vertices.size())
		{
			error("VertexArray: Color and Vertex count mismatch!");
			glBindVertexArray(0);
			return;
		}
		GLBuffer cols = GLBuffer(colors.data(), sizeof(colors), colors.size());
		cols.bind();
		cols.data_pointer(SHADER_COLOR_LOCATION, 4, GL_FLOAT, 4 * sizeof(float), true, BUFFER_OFFSET(0));
		glbuffers.emplace_back(cols);
	}

	//
	//	UVs
	//
	if(texcoords.size() > 0)
	{
		if(texcoords.size() != vertices.size())
		{
			error("VertexArray: UV and Vertex count mismatch!");
			glBindVertexArray(0);
			return;
		}
		GLBuffer tex = GLBuffer(texcoords.data(), sizeof(texcoords), texcoords.size());
		tex.bind();
		tex.data_pointer(SHADER_UV_LOCATION, 2, GL_FLOAT, 2 * sizeof(float), false, BUFFER_OFFSET(0));
		glbuffers.emplace_back(tex);
	}


}

void VertexArray::clear()
{
	vertices.clear();
	normals.clear();
	colors.clear();
	texcoords.clear();

	// Do buffer clearing stuff
	for(GLBuffer it : glbuffers)
	{
		it.clear(); // Frees/releases the underlying data for this buffer
	}

	glbuffers.clear();
}

void VertexArray::bind()
{
	if(vao != 0)
		glBindVertexArray(vao);
}

void VertexArray::unbind()
{
	glBindVertexArray(0);
}

