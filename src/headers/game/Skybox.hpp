#ifndef _SKYBOX_HPP_
#define _SKYBOX_HPP_


#include "graphics/Model.hpp"
#include <string>

class GLTexture;
class TextureManager;

class Skybox : public Model {
public:
	Skybox();
	~Skybox();

	void set_skybox(std::string path, TextureManager* tex_man);

	void dispose() override;

private:

	GLTexture* skybox_tex;

};

#endif