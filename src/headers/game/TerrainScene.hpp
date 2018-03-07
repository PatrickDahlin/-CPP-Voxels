#ifndef _TERRAINSCENE_HPP_
#define _TERRAINSCENE_HPP_

#include "core/Common.hpp"
#include "core/Scene.hpp"

#include "game/DebugCamera.hpp"


class ShaderProgram;
class Model;

class Input;
class SceneManager;
class ShaderManager;
class TextureManager;
class RenderPass;

class Terrain;

class TerrainScene : public Scene {
public:
	TerrainScene(Input* input, SceneManager* scene_manager);
	~TerrainScene();

	void init() override;

	void load(ShaderManager& sha_man, TextureManager& tex_man) override;

	void update(const float delta) override;

	void render(RenderPass& pass) override;

	void resized_window(int width, int height) override;

	void unload() override;

	void dispose() override;

private:

	ShaderProgram*		skybox_shader;
	Model*				skybox_model;

	DebugCamera			debug_camera;

	Terrain*			voxel_terrain;

};

#endif