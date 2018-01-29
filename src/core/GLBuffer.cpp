#include "../headers/core/GLBuffer.hpp"
#include "../headers/core/Macros.hpp"
#include <GL/glew.h>

GLBuffer::GLBuffer(void *data, unsigned int size_bytes,
				   unsigned int vertex_count) : element_count(vertex_count),
												size_bytes(size_bytes),
												vbo_id(0),
												attribute_count(0)
{
	uploadData(data, size_bytes, vertex_count);
}

GLBuffer::~GLBuffer()
{

	/*
	
		TODO: if dispose is called here the buffer is prematurely deleted
		This will cause an instant crash but no error is reported, why?
	
	*/

	//dispose();
}

void GLBuffer::dispose()
{
	glDeleteBuffers(1, &vbo_id);
	element_count = 0;
	size_bytes = 0;
}

void GLBuffer::uploadData(void *data, unsigned int size_bytes, unsigned int vertex_count)
{
	this->size_bytes = size_bytes;
	this->element_count = element_count;

	if (vbo_id == 0)
		glGenBuffers(1, &vbo_id);

	bind();

	clear();
	
	glBufferData(GL_ARRAY_BUFFER, size_bytes, data, GL_STATIC_DRAW);

	unbind();
}

void GLBuffer::clear()
{
	if(vbo_id == 0) return;

	bind();

	glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW); // We don't care about old data, if any, that driver can discard

	//unbind();
}

void GLBuffer::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
	if(vbo_id == 0) error("bound glbuffer with 0 id");
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
