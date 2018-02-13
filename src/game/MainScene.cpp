#include "game/MainScene.hpp"
#include "graphics/Primitives.hpp"
#include "graphics/RenderPass.hpp"
#include "graphics/ShaderProgram.hpp"
#include "game/DebugCamera.hpp"
#include "core/Files.hpp"
#include "graphics/Material.hpp"
#include "core/Macros.hpp"

#include "graphics/OrthographicCamera.hpp"

#include <imgui/imgui.h>
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
	cam = new DebugCamera(60.0f, 1280, 720, 0.1f, 500.0f);
	cam->set_input(input);
	cam->set_fly_speed(3.0f);
	cam->set_mouse_sensitivity(0.5f);

	std::string vert = read_file("data/shaders/Basic-vert.glsl");
	std::string frag = read_file("data/shaders/Basic-frag.glsl");
	std::string header = read_file("data/shaders/Shader_Header.glsl");
	shader = new ShaderProgram(vert.c_str(), frag.c_str(), header.c_str());

	tmp = Primitives::create_cube();

	mat = new Material();
	mat->texture = load_image("data/textures/grass.jpg");
	
	tmp->set_material(mat);
}

void MainScene::load()
{}

void MainScene::unload()
{}

void MainScene::update(const float delta)
{
	cam->update(delta);

	ImGui::Begin("MainScene info");
	ImGui::Text("Deltatime %.4f", delta);
	ImGui::Text("FPS: %.0f",(1.0f / delta));
	ImGui::End();
}

void MainScene::render(RenderPass* pass)
{
	pass->draw_model(tmp, shader, cam);

	bool val = true;
	
	ImGui::ShowDemoWindow(&val);

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



