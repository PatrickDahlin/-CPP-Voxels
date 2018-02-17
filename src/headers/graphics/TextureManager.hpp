#ifndef _TEXTUREMANAGER_HPP_
#define _TEXTUREMANAGER_HPP_

#include "core/ResourceManager.hpp"
#include <string>
#include "graphics/GLTexture.hpp"

class TextureManager : public ResourceManager<std::string, GLTexture> {
public:
	TextureManager();
	~TextureManager();

	GLTexture* get_texture(std::string path, ColorFormat format);

	static TextureManager* get_instance();

private:

	static TextureManager* instance;

};

#endif