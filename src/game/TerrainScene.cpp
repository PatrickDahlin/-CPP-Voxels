#include "game/TerrainScene.hpp"

#include "graphics/ShaderManager.hpp"
#include "graphics/TextureManager.hpp"
#include "graphics/RenderPass.hpp"
#include "core/Input.hpp"


#include "graphics/Model.hpp"
#include "graphics/Material.hpp"


TerrainScene::TerrainScene(Input* input, SceneManager* scene_manager) : Scene(input, scene_manager),
skybox_shader(nullptr),
skybox_model(nullptr),
debug_camera(60.0f, 1280, 720, 0.1f, 500.0f),
voxel_terrain()
{
	debug_camera.set_input(input);
	voxel_terrain.set_center(debug_camera.get_position());
}

TerrainScene::~TerrainScene()
{}

void TerrainScene::init()
{}

void TerrainScene::load(ShaderManager& sha_man, TextureManager& tex_man)
{
	skybox_shader = sha_man.get_shader("data/shaders/Skybox-vert.glsl","data/shaders/Skybox-frag.glsl");
	skybox_model = load_obj_from_file("data/models/cubemapped_cube.obj");
	skybox_model->get_material()->texture = tex_man.get_texture("data/textures/Skybox.png", ColorFormat::RGB);
	skybox_model->transform.scale(100.0f,100.0f,100.0f);
}

void TerrainScene::update(const float delta)
{
	if(input->is_mouse_locked() != debug_camera.is_mouse_input_active())
		debug_camera.set_mouse_input_active(input->is_mouse_locked());

	debug_camera.update(delta);

	voxel_terrain.set_center(debug_camera.get_position());
	voxel_terrain.update(delta);
}

void TerrainScene::render(RenderPass& pass)
{
	
	pass.draw_model(skybox_model, skybox_shader, &debug_camera);

	voxel_terrain.render(pass);
}

void TerrainScene::resized_window(int width, int height)
{
	vec3 lastpos = debug_camera.get_position();
	vec3 lastrot = debug_camera.get_euler();

	debug_camera = DebugCamera(60.0f, width, height, 0.1f, 500.0f);
	debug_camera.set_input(input);
	debug_camera.set_position(lastpos);
	debug_camera.set_rotation(lastrot);
}

void TerrainScene::unload()
{}

void TerrainScene::dispose()
{
	voxel_terrain.dispose();
}

