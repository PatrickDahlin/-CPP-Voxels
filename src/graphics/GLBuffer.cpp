#include "graphics/GLBuffer.hpp"
#include "core/Macros.hpp"
#include <GL/glew.h>

GLBuffer::GLBuffer(BufferType buf_type, void *data, unsigned int size_bytes,
				   unsigned int vertex_count) : 
type(buf_type),
element_count(vertex_count),
size_bytes(size_bytes),
vbo_id(0),
attribute_count(0)
{
	uploadData(data, size_bytes, vertex_count);
}

GLBuffer::~GLBuffer()
{}

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
	
	if(type == BufferType::ELEMENT)
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_bytes, data, GL_STATIC_DRAW);
	else
		glBufferData(GL_ARRAY_BUFFER, size_bytes, data, GL_STATIC_DRAW);
	
}

void GLBuffer::clear()
{
	if(type == BufferType::ELEMENT)
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW); // We don't care about old data, if any, that driver can discard	
	else
		glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_STATIC_DRAW); // We don't care about old data, if any, that driver can discard	
		
}

void GLBuffer::bind()
{
	if(type == BufferType::ELEMENT)
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_id);
	else
		glBindBuffer(GL_ARRAY_BUFFER, vbo_id);

	assert(vbo_id);
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
	if(type == BufferType::ARRAY)
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

int GLBuffer::get_size()
{
	return element_count;
}