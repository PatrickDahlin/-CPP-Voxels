#ifndef _MAINSCENE_HPP_
#define _MAINSCENE_HPP_

#include "core/Scene.hpp"

class RenderPass;
class Input;
class SceneManager;
class Model;
class Camera;
class ShaderProgram;
class Material;
class GLTexture;

class MainScene : public Scene
{
public:
    MainScene(Input* input, SceneManager* scene_manager);
    ~MainScene();

	void init() override;

    void load() override;

	void unload() override;

    void update(const float delta) override;

    void render(RenderPass* pass) override;

    void dispose() override;

protected:


	Model*			tmp;
	Camera*			cam;
	ShaderProgram* 	shader;
	Material*		mat;
	GLTexture*		tex;

};


#endif