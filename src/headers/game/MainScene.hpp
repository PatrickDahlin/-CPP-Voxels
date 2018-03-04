#ifndef _MAINSCENE_HPP_
#define _MAINSCENE_HPP_

#include "core/Scene.hpp"


// Required for Scene
class ShaderManager;
class TextureManager;
class RenderPass;
class Input;
class SceneManager;
// ------------------

class Model;
class DebugCamera;
class ShaderProgram;
class Material;
class GLTexture;
class Camera;

class VoxelData;
class VertexArray;

class MainScene : public Scene
{
public:
    MainScene(Input* input, SceneManager* scene_manager);
    ~MainScene();

	void init() override;

    void load(ShaderManager& sha_man, TextureManager& tex_man) override;

	void unload() override;

    void update(const float delta) override;

    void render(RenderPass& pass) override;

	void resized_window(int width, int height) override;

    void dispose() override;

protected:


	Model*			tmp;
    DebugCamera*	cam;
	ShaderProgram* 	shader;
	Material*		mat;
	GLTexture*		tex;
	
	VoxelData*		myvoxels;
	Model*			voxel_model;
	ShaderProgram*	voxel_shader;
	ShaderProgram*	skybox_shader;
	Model*			skybox;
	Model*			test_loaded_model;

};


#endif