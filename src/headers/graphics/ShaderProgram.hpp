#ifndef _SHADERPGROGRAM_HPP_
#define _SHADERPGROGRAM_HPP_

#include <string>
#include <GL/glew.h>
#include <glm/mat4x4.hpp>



class ShaderProgram
{
public:
	ShaderProgram(std::string vert_src, std::string frag_src, std::string header);
	~ShaderProgram();

	void use();

	void upload_projection(glm::mat4 proj);
	void upload_view(glm::mat4 view);
	void upload_model(glm::mat4 model);

	void set_bool(const std::string& name, bool val) const;
	void set_int(const std::string& name, int val) const;
	void set_float(const std::string& name, float val) const;

protected:

	void load_shaders(std::string v_src, std::string f_src, std::string header);

	void load_uniform_locations();

	GLuint		shader_program;

	GLuint		mat4_proj_loc;
	GLuint		mat4_view_loc;
	GLuint		mat4_model_loc;

};

#endif