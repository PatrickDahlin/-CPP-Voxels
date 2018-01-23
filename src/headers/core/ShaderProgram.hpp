#ifndef _SHADERPGROGRAM_HPP_
#define _SHADERPGROGRAM_HPP_

#include <GL/glew.h>

class ShaderProgram
{
public:
	ShaderProgram(const char* vert_src, const char* frag_src);
	~ShaderProgram();

	void use();

protected:

	void load_shaders(const char* v_src, const char* f_src);

	GLuint		shader_program;

};

#endif