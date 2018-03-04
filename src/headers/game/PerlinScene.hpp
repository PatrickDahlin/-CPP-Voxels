#ifndef _PERLINSCENE_HPP_
#define _PERLINSCENE_HPP_

#include "core/Scene.hpp"

class Input;
class SceneManager;
class ShaderManager;
class TextureManager;


class GLTexture;
class Material;
class Model;
class DebugCamera;
class ShaderProgram;

class PerlinScene : public Scene {
public:
	PerlinScene(Input* input, SceneManager* scn_man);
	~PerlinScene();

	void load(ShaderManager& sha_man, TextureManager& tex_man) override;

	void init() override;

	void unload() override;

	void update(const float delta) override;

	void render(RenderPass& pass) override;

	void resized_window(int width, int height) override;

	void dispose() override;

protected:

	GLTexture* perlin_tex;
	Material* perlin_mat;
	Model* perlin_model;
	DebugCamera* debug_cam;
	ShaderProgram* perlin_shader;

};


#endif