#include "game/TerrainScene.hpp"

#include "graphics/ShaderManager.hpp"
#include "graphics/TextureManager.hpp"
#include "graphics/RenderPass.hpp"
#include "core/Input.hpp"
#include "terrain/Terrain.hpp"

#include "graphics/Model.hpp"
#include "graphics/Material.hpp"
#include <imgui/imgui.h>


TerrainScene::TerrainScene(Input* input, SceneManager* scene_manager) : Scene(input, scene_manager),
skybox_shader(nullptr),
skybox_model(nullptr),
debug_camera(60.0f, 1280, 720, 1.0f, 1000.0f),
voxel_terrain(nullptr)
{
	debug_camera.set_input(input);
	debug_camera.set_fly_speed(5);
}

TerrainScene::~TerrainScene()
{}

void TerrainScene::init()
{
	voxel_terrain = new Terrain();
	printf("Initialize voxel terrain\n");
	voxel_terrain->init();
	voxel_terrain->set_draw_dist(4);
}

void TerrainScene::load(ShaderManager& sha_man, TextureManager& tex_man)
{
	skybox_shader = sha_man.get_shader("data/shaders/Skybox-vert.glsl","data/shaders/Skybox-frag.glsl");
	skybox_model = load_obj_from_file("data/models/cubemapped_cube.obj");
	skybox_model->get_material()->texture = tex_man.get_texture("data/textures/Skybox.png", ColorFormat::RGB);
	skybox_model->transform.scale(100.0f,100.0f,100.0f);
}

void TerrainScene::update(const float delta)
{
	assert(voxel_terrain);
	if(input->is_mouse_locked() != debug_camera.is_mouse_input_active())
		debug_camera.set_mouse_input_active(input->is_mouse_locked());

	debug_camera.update(delta);

	static bool update_center = true;

	if(update_center)
		voxel_terrain->set_center(debug_camera.get_position());
	
	if(input->get_key(SDLK_f))
		update_center = !update_center;

	if(input->get_key_down(SDLK_LSHIFT))
		debug_camera.set_fly_speed(100);
	else
		debug_camera.set_fly_speed(50);
		
	static int draw_dist = 5;
	ImGui::Begin("Terrain");
	ImGui::InputInt("Draw distance", &draw_dist, 1,20);
	ImGui::End();

	voxel_terrain->set_draw_dist(draw_dist);

	voxel_terrain->update(delta);
}

void TerrainScene::render(RenderPass& pass)
{
	assert(voxel_terrain);
	pass.switch_to_cam(&debug_camera);

	// Disable depth mask to make sure skybox doesn't cover up any objects
	glDepthMask(GL_FALSE);
	pass.do_instant_render(skybox_model, skybox_shader, &debug_camera);
	glDepthMask(GL_TRUE);
	
	voxel_terrain->render(pass);
}

void TerrainScene::resized_window(int width, int height)
{
	vec3 lastpos = debug_camera.get_position();
	vec3 lastrot = debug_camera.get_euler();

	debug_camera = DebugCamera(60.0f, width, height, 1.0f, 1000.0f);
	debug_camera.set_fly_speed(30);
	debug_camera.set_input(input);
	debug_camera.set_position(lastpos);
	debug_camera.set_rotation(lastrot);
}

void TerrainScene::unload()
{}

void TerrainScene::dispose()
{
	assert(voxel_terrain);
	voxel_terrain->dispose();
	delete voxel_terrain;
}

