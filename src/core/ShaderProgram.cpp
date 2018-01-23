#include "../headers/core/ShaderProgram.hpp"
#include "../headers/core/Errors.hpp"
#include <vector>

ShaderProgram::ShaderProgram(const char* vert_src, const char* frag_src) :
shader_program(0), vert_shader_src(vert_src), frag_shader_src(frag_src)
{
	load_shaders();	
}

ShaderProgram::~ShaderProgram()
{
	if(shader_program)
	{
		glDeleteProgram(shader_program);
	}
	delete vert_shader_src;
	delete frag_shader_src;
}

void ShaderProgram::load_shaders()
{
	GLuint vert_shader_id = glCreateShader(GL_VERTEX_SHADER);
	GLuint frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	GLint result = GL_FALSE;
	int info_log_length = 0;

	std::cout << "Compiling vertex shader..." << std::endl;

	glShaderSource(vert_shader_id, 1, &vert_shader_src , NULL);
	glCompileShader(vert_shader_id);

	glGetShaderiv(vert_shader_id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vert_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
	if ( info_log_length > 0 ){
		std::vector<char> VertexShaderErrorMessage(info_log_length+1);
		glGetShaderInfoLog(vert_shader_id, info_log_length, NULL, &VertexShaderErrorMessage[0]);
		error(VertexShaderErrorMessage.data());
		glDeleteShader(vert_shader_id);
		return;
	}


	info_log_length = 0;
	result = GL_FALSE;

	std::cout << "Compiling fragment shader..." << std::endl;

	glShaderSource(frag_shader_id, 1, &frag_shader_src , NULL);
	glCompileShader(frag_shader_id);

	glGetShaderiv(frag_shader_id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(frag_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
	if ( info_log_length > 0 ){
		std::vector<char> FragmentShaderErrorMessage(info_log_length+1);
		glGetShaderInfoLog(frag_shader_id, info_log_length, NULL, &FragmentShaderErrorMessage[0]);
		error(FragmentShaderErrorMessage.data());
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
		error(ProgramErrorMessage.data());
		glDeleteProgram(shader_program);
	}
	else
		std::cout << "Successfully linked program" << std::endl;

	glDetachShader(shader_program, frag_shader_id);
	glDetachShader(shader_program, vert_shader_id);

	glDeleteShader(frag_shader_id);
	glDeleteShader(vert_shader_id);
}


void ShaderProgram::use()
{
	glUseProgram(shader_program);
}

