#include "graphics/ShaderManager.hpp"
#include "core/Files.hpp"
#include "graphics/ShaderProgram.hpp"
#include "core/Macros.hpp"

ShaderManager::ShaderManager() : ResourceManager()
{}

ShaderManager::~ShaderManager()
{}

ShaderProgram* ShaderManager::get_shader(std::string vert, std::string frag)
{

	ShaderParams key(vert,frag);
	if(ResourceManager::has_resource(key))
	{
		coutln("Found existing shader and returning that in ShaderManager");
		return ResourceManager::get_resource(key);
	}
	else
	{
		coutln("Loaded new shader in ShaderManager");
		std::string v_src = read_file(vert.c_str());
		std::string f_src = read_file(frag.c_str());
		std::string head = read_file("data/shaders/Shader_Header.glsl");
		ShaderProgram* shader = new ShaderProgram(v_src.c_str(), f_src.c_str(), head.c_str());
		ResourceManager::add_resource(key, shader);
		return shader;
	}
}
