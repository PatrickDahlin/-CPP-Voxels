#ifndef _SHADERPGROGRAM_HPP_
#define _SHADERPGROGRAM_HPP_

#include <string>
#include <GL/glew.h>

class ShaderProgram
{
public:
	ShaderProgram(std::string vert_src, std::string frag_src);
	~ShaderProgram();

	void use();

protected:

	void load_shaders(std::string v_src, std::string f_src);

	GLuint		shader_program;

};

#endif