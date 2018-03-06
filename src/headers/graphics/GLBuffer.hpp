#ifndef _GLBUFFER_HPP_
#define _GLBUFFER_HPP_

#include <GL/glew.h>

/*
Important!
Since OpenGL 3.3 there is no default VAO bound (zero buffer/array)
And to draw a VBO you NEED to bind a VAO
*/

enum class BufferType { ARRAY, ELEMENT };

class GLBuffer
{
  public:
	GLBuffer(BufferType buf_type, void *data, unsigned int size_bytes, unsigned int vertex_count);
	~GLBuffer();

	/* Define a vertex attribute pointer for the data, !not stored! so call this
	   on each rendercall
	   Type must be one of GL_FLOAT, GL_INT, GL_UNSIGNED_* etc. 
	*/
	void data_pointer(int location,
					  unsigned int element_per_vertex,
					  unsigned int type,
					  unsigned int stride,
					  bool normalized,
					  void* offset);

	/* Bind this buffer to GL_ARRAY_BUFFER slot */
	void bind();

	void clear();

	/* Unbind this buffer and it's attribute arrays */
	void unbind();
	
	/* Draws this buffer, expecting data to contain at least vertices
	   Note that data_pointer needs to be called first to define layout */
	void draw();

	void uploadData(void *data, unsigned int size_bytes, unsigned int vertex_count);

	void dispose();

	int get_element_count();

  protected:

	BufferType type;

	unsigned int element_count;
	unsigned int size_bytes;
	unsigned int vbo_id;

	int enabled_attributes[15];
	int attribute_count;
};

#endif