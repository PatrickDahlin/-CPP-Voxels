#include "graphics/ShaderManager.hpp"
#include "core/Files.hpp"
#include "graphics/ShaderProgram.hpp"

ShaderManager* ShaderManager::instance = nullptr;

ShaderManager::ShaderManager() : ResourceManager()
{}

ShaderManager::~ShaderManager()
{}

void ShaderManager::dispose()
{
	printf("Destroying shadermanager\n");
	if(instance)
	{
		instance->dispose();
	}
	instance = nullptr;
}

ShaderProgram* ShaderManager::get_shader(std::string vert, std::string frag)
{
	if(!instance) instance = new ShaderManager();

	ShaderParams key(vert,frag);
	if(instance->ResourceManager::has_resource(key))
	{
		return instance->ResourceManager::get_resource(key);
	}
	else
	{
		std::string v_src = read_file(vert.c_str());
		std::string f_src = read_file(frag.c_str());
		std::string head = read_file("data/shaders/Shader_Header.glsl");
		ShaderProgram* shader = new ShaderProgram(v_src.c_str(), f_src.c_str(), head.c_str());
		instance->ResourceManager::add_resource(key, shader);
		return shader;
	}
}
