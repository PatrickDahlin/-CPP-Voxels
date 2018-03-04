#include "graphics/ShaderProgram.hpp"
#include "graphics/Shader_vars.hpp"
#include "core/Common.hpp"
#include "core/Files.hpp"
#include <vector>
#include <glm/gtc/type_ptr.hpp>

ShaderProgram::ShaderProgram(std::string vert_src, std::string frag_src, std::string header) :
shader_program(0),
mat4_proj_loc(0),
mat4_view_loc(0),
mat4_model_loc(0)
{
	load_shaders(vert_src, frag_src, header);	
}

ShaderProgram::~ShaderProgram()
{
}

void ShaderProgram::dispose()
{
	if(shader_program)
	{
		glDeleteProgram(shader_program);
	}
}

void ShaderProgram::load_shaders(std::string v_src, std::string f_src, std::string header)
{

	v_src = header + v_src;
	f_src = header + f_src;

	GLuint vert_shader_id = glCreateShader(GL_VERTEX_SHADER);
	GLuint frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	GLint result = GL_FALSE;
	int info_log_length = 0;

	std::cout << "Compiling vertex shader..." << std::endl;

	const char* tmp = v_src.data();
	glShaderSource(vert_shader_id, 1, &tmp, NULL);
	glCompileShader(vert_shader_id);

	glGetShaderiv(vert_shader_id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vert_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
	if ( info_log_length > 0 ){
		std::vector<char> VertexShaderErrorMessage(info_log_length+1);
		glGetShaderInfoLog(vert_shader_id, info_log_length, NULL, &VertexShaderErrorMessage[0]);
		printf("Frag shader compile error: %s\n",VertexShaderErrorMessage.data());
		glDeleteShader(vert_shader_id);
		return;
	}


	info_log_length = 0;
	result = GL_FALSE;

	std::cout << "Compiling fragment shader..." << std::endl;

	tmp = f_src.data();
	glShaderSource(frag_shader_id, 1, &tmp , NULL);
	glCompileShader(frag_shader_id);

	glGetShaderiv(frag_shader_id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(frag_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
	if ( info_log_length > 0 ){
		std::vector<char> FragmentShaderErrorMessage(info_log_length+1);
		glGetShaderInfoLog(frag_shader_id, info_log_length, NULL, &FragmentShaderErrorMessage[0]);
		printf("Frag shader compile error: %s\n",FragmentShaderErrorMessage.data());
		glDeleteShader(vert_shader_id);
		glDeleteShader(frag_shader_id);
		return;
	}


	std::cout << "Linking shader program..." << std::endl;

	// Link program
	shader_program = glCreateProgram();
	glAttachShader(shader_program, vert_shader_id);
	glAttachShader(shader_program, frag_shader_id);

	glLinkProgram(shader_program);


	glGetProgramiv(shader_program, GL_LINK_STATUS, &result);
	glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &info_log_length);
	if ( info_log_length > 0 ){
		std::vector<char> ProgramErrorMessage(info_log_length+1);
		glGetProgramInfoLog(shader_program, info_log_length, NULL, &ProgramErrorMessage[0]);
		printf("Shader program link error: %s\n",ProgramErrorMessage.data());
		glDeleteProgram(shader_program);
		glDeleteShader(vert_shader_id);
		glDeleteShader(frag_shader_id);
		return;
	}
	else
		std::cout << "Successfully linked program" << std::endl;

	glDetachShader(shader_program, frag_shader_id);
	glDetachShader(shader_program, vert_shader_id);

	glDeleteShader(frag_shader_id);
	glDeleteShader(vert_shader_id);

	CHECK_GL_ERROR();

	load_uniform_locations();
}

void ShaderProgram::load_uniform_locations()
{
	assert(shader_program);
	
	mat4_proj_loc  = glGetUniformLocation(shader_program, SHADER_PROJECTIONMAT_UNIFORM_NAME);
	mat4_view_loc  = glGetUniformLocation(shader_program, SHADER_VIEWMAT_UNIFORM_NAME);
	mat4_model_loc = glGetUniformLocation(shader_program, SHADER_MODELMAT_UNIFORM_NAME);


	//printf("proj loc:%i\nview loc:%i\nmodel loc:%i\n",mat4_proj_loc,mat4_view_loc,mat4_model_loc);

	CHECK_GL_ERROR();
}

void ShaderProgram::upload_projection(glm::mat4 proj)
{
	assert(shader_program);
	glUniformMatrix4fv(mat4_proj_loc, 1, GL_FALSE, glm::value_ptr(proj));
}

void ShaderProgram::upload_view(glm::mat4 view)
{
	assert(shader_program);
	glUniformMatrix4fv(mat4_view_loc, 1, GL_FALSE, glm::value_ptr(view));
}

void ShaderProgram::upload_model(glm::mat4 model)
{
	assert(shader_program);
	glUniformMatrix4fv(mat4_model_loc, 1, GL_FALSE, glm::value_ptr(model));
}

void ShaderProgram::use()
{
	assert(shader_program);
	glUseProgram(shader_program);
}

void ShaderProgram::set_bool(const std::string& name, bool val) const
{
	assert(shader_program);
	glUniform1i(glGetUniformLocation(shader_program, name.c_str()), (int)val );
}

void ShaderProgram::set_int(const std::string& name, int val) const
{
	assert(shader_program);
	glUniform1i(glGetUniformLocation(shader_program, name.c_str()), val);
}

void ShaderProgram::set_float(const std::string& name, float val) const
{
	assert(shader_program);
	glUniform1f(glGetUniformLocation(shader_program, name.c_str()), val);
}

