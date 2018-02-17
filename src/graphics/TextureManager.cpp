#include "graphics/TextureManager.hpp"
#include "core/ResourceManager.hpp"

TextureManager* TextureManager::instance = nullptr;

TextureManager::TextureManager() : ResourceManager()
{
}

TextureManager::~TextureManager()
{
}

GLTexture* TextureManager::get_texture(std::string path, ColorFormat format)
{
	if(ResourceManager::has_resource(path))
	{
		return ResourceManager::get_resource(path);
	}
	else
	{
		GLTexture* tex = load_image(path.c_str(), format);
		ResourceManager::add_resource(path, tex);
		return tex;
	}
}

TextureManager* TextureManager::get_instance()
{
	if(!instance) instance = new TextureManager();
	return instance;
}
