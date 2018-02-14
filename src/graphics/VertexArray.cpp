#include "graphics/VertexArray.hpp"
#include "graphics/GLBuffer.hpp"
#include "core/Macros.hpp"
#include <GL/glew.h>


VertexArray::VertexArray() :
vertices(),
normals(),
colors(),
texcoords(),
vao(0),
glbuffers(),
element_buffer(nullptr)
{

	glGenVertexArrays(1, &vao);
	
}

VertexArray::~VertexArray()
{
}

void VertexArray::dispose()
{
	for(GLBuffer& it : glbuffers)
		it.dispose();

	glDeleteVertexArrays(1, &vao);
}

void VertexArray::upload_data()
{
	// Clean up old buffers
	for(GLBuffer& it : glbuffers)
		it.dispose();

	glbuffers.clear();

	glBindVertexArray(vao);
	
	//
	//	Vertices
	//
	if(vertices.size() > 0)
	{
		GLBuffer verts = GLBuffer(BufferType::ARRAY, vertices.data(), vertices.size() * 3 * sizeof(float), vertices.size());
		verts.data_pointer(SHADER_POSITION_LOCATION, 3, GL_FLOAT, 3 * sizeof(float), false, BUFFER_OFFSET(0));
		glbuffers.emplace_back(verts);
	}
	else 
	{
		error("No vertices to upload");
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
			return;
		}
		GLBuffer norms = GLBuffer(BufferType::ARRAY, normals.data(), normals.size() * 3 * sizeof(float), normals.size());
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
			return;
		}
		GLBuffer cols = GLBuffer(BufferType::ARRAY, colors.data(), colors.size() * 4 * sizeof(float), colors.size());
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
			return;
		}
		GLBuffer tex = GLBuffer(BufferType::ARRAY, texcoords.data(), texcoords.size() * 2 * sizeof(float), texcoords.size());
		tex.data_pointer(SHADER_UV_LOCATION, 2, GL_FLOAT, 2 * sizeof(float), false, BUFFER_OFFSET(0));
		glbuffers.emplace_back(tex);
	}


}

void VertexArray::add_element_buffer(GLBuffer* buf)
{
	if(element_buffer != nullptr)
	{
		element_buffer->dispose();
		delete element_buffer;
	}
	element_buffer = buf;
}

void VertexArray::add_buffer(GLBuffer buf)
{
	glbuffers.emplace_back(buf);
}

void VertexArray::clear()
{
	vertices.clear();
	normals.clear();
	colors.clear();
	texcoords.clear();

	// Do buffer clearing stuff
	for(GLBuffer& it : glbuffers)
	{
		it.clear(); // Frees/releases the underlying data for this buffer
	}

	if(element_buffer != nullptr)
	{
		element_buffer->dispose();
		delete element_buffer;
		element_buffer = nullptr;
	}

	glbuffers.clear();
}

void VertexArray::draw(RenderMode mode)
{
	if(vertices.size() <= 0) return;

	bind();

	CHECK_GL_ERROR();

	GLenum gl_mode = GL_TRIANGLES;
	switch(mode)
	{
		case RenderMode::LINE:
			gl_mode = GL_LINE;
			break;
		case RenderMode::POINT:
			gl_mode = GL_POINT;
			break;
		case RenderMode::TRIANGLE_STRIP:
			gl_mode = GL_TRIANGLE_STRIP;
			break;
		case RenderMode::TRIANGLE_FAN:
			gl_mode = GL_TRIANGLE_FAN;
			break;
		case RenderMode::TRIANGLE:
		default:
			gl_mode = GL_TRIANGLES;
			break;
	}
	

	if(element_buffer != nullptr)
		glDrawElements(gl_mode, element_buffer->get_size(), GL_INT, (void*)0);
	else
		glDrawArrays(gl_mode, 0, vertices.size());

	CHECK_GL_ERROR();


}

void VertexArray::bind()
{
	if(vao != 0)
		glBindVertexArray(vao);

	if(vao == 0) error("tried to bind vertexarray with 0 id");
}

void VertexArray::unbind()
{
	glBindVertexArray(0);
}

void VertexArray::set_vertices(vector<vec3> verts)
{
	vertices = verts;
}

void VertexArray::set_normals(vector<vec3> norms)
{
	normals = norms;
}

void VertexArray::set_colors(vector<vec4> cols)
{
	colors = cols;
}

void VertexArray::set_texcoords(vector<vec2> coords)
{
	texcoords = coords;
}

void VertexArray::add_vertex(vec3 vert)
{
	vertices.emplace_back(vert);
}

void VertexArray::add_vertex(float x, float y, float z)
{
	vertices.emplace_back(x, y, z);
}

void VertexArray::add_normal(vec3 norm)
{
	normals.emplace_back(norm);
}

void VertexArray::add_normal(float x, float y, float z)
{
	normals.emplace_back(x, y, z);
}

void VertexArray::add_color(vec4 color)
{
	colors.emplace_back(color);
}

void VertexArray::add_color(float r, float g, float b, float a)
{
	colors.emplace_back(r, g, b, a);
}

void VertexArray::add_texcoord(vec2 coord)
{
	texcoords.emplace_back(coord);
}

void VertexArray::add_texcoord(float u, float v)
{
	texcoords.emplace_back(u, v);
}