#include "../headers/core/GLBuffer.hpp"
#include <GL/glew.h>

GLBuffer::GLBuffer(void *data, unsigned int size_bytes,
				   unsigned int vertex_count) : element_count(vertex_count),
												size_bytes(size_bytes),
												vbo_id(0),
												attribute_count(0)
{
	uploadData(data);
}

GLBuffer::~GLBuffer()
{
	glDeleteBuffers(1, &vbo_id);
	element_count = 0;
	size_bytes = 0;
}

void GLBuffer::uploadData(void *data)
{
	if (vbo_id == 0)
		glGenBuffers(1, &vbo_id);

	bind();

	glBufferData(GL_ARRAY_BUFFER, size_bytes, data, GL_STATIC_DRAW);
}

void GLBuffer::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
}

void GLBuffer::unbind()
{
	for(int i = attribute_count-1; i >= 0; i--)
	{
		glDisableVertexAttribArray(enabled_attributes[i]);
	}
	
	attribute_count = 0;
}

void GLBuffer::draw()
{
	glDrawArrays(GL_TRIANGLES, 0, element_count);
}

void GLBuffer::data_pointer(int location,
							unsigned int element_per_vertex,
							unsigned int type,
							unsigned int stride,
							bool normalized,
							void* offset)
{
	glEnableVertexAttribArray(location);
	glVertexAttribPointer(
		location,	  // Attrib *location*, must match layout in shader
		element_per_vertex,		   // size, aka element count per vertex
		type,		   // type
		(normalized ? GL_TRUE : GL_FALSE),	  // normalized?
		stride,		   // stride
		(void*)0 // offset
	);

	enabled_attributes[attribute_count++] = location;
}
