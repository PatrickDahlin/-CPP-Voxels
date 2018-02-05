#include "game/MainScene.hpp"
#include "graphics/Primitives.hpp"
#include "graphics/RenderPass.hpp"
#include "graphics/ShaderProgram.hpp"
#include "game/DebugCamera.hpp"
#include "core/Files.hpp"
#include "graphics/Material.hpp"
#include "core/Macros.hpp"

#include <stb_image.h>


MainScene::MainScene(Input* input, SceneManager* scene_manager) : Scene(input, scene_manager),
cam(nullptr),
shader(nullptr),
mat(nullptr)
{
}

MainScene::~MainScene()
{}

void MainScene::init()
{
	cam = new DebugCamera(3.14f / 4.0f, 1280, 720, 0.1f, 500.0f);
	cam->set_input(input);
	cam->set_fly_speed(3.0f);
	cam->set_mouse_sensitivity(0.05f);

	std::string vert = read_file("data/shaders/Basic-vert.glsl");
	std::string frag = read_file("data/shaders/Basic-frag.glsl");
	std::string header = read_file("data/shaders/Shader_Header.glsl");
	shader = new ShaderProgram(vert.c_str(), frag.c_str(), header.c_str());

	tmp = Primitives::create_cube();

	mat = new Material();
	int width, height, channelnr;
	width = 0;
	height = 0;
	unsigned char* data = stbi_load("data/textures/grass.jpg", &width, &height, &channelnr, 0);
	GLTexture* myTexture = new GLTexture(data, width, height);
	mat->texture = myTexture;
	stbi_image_free(data);

	tmp->set_material(mat);
}

void MainScene::load()
{}

void MainScene::unload()
{}

void MainScene::update(const float delta)
{
	cam->update(delta);
}

void MainScene::render(RenderPass* pass)
{
	pass->draw_model(tmp, shader, cam);
}

void MainScene::dispose()
{
	Scene::dispose();
	tmp->dispose();
	mat->dispose();
	delete tmp;
	delete mat;
	delete shader;
	delete cam;
}



