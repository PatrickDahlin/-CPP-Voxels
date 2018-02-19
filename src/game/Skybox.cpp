#include "game/Skybox.hpp"
#include "graphics/GLTexture.hpp"
#include "graphics/TextureManager.hpp"
#include "graphics/Material.hpp"

Skybox::Skybox() : skybox_tex(0)
{
	set_material(new Material());
}

Skybox::~Skybox()
{}

void Skybox::dispose()
{
	Model::dispose();
	if (get_material()) delete get_material();
}

void Skybox::set_skybox(std::string path, TextureManager* tex_man)
{
	skybox_tex = tex_man->get_texture(path, ColorFormat::RGB);
	get_material()->texture = skybox_tex;
}