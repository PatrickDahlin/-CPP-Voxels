#include "game/PerlinScene.hpp"
#include "core/Input.hpp"
#include "core/SceneManager.hpp"
#include "graphics/RenderPass.hpp"
#include "graphics/ShaderManager.hpp"
#include "graphics/TextureManager.hpp"


#include "game/DebugCamera.hpp"
#include "graphics/GLTexture.hpp"
#include "graphics/Material.hpp"
#include "graphics/Model.hpp"
#include "graphics/Primitives.hpp"
#include "graphics/ShaderProgram.hpp"
#include "core/PerlinNoise.hpp"
#include "core/Time.hpp"

#include <algorithm>


PerlinScene::PerlinScene(Input* input, SceneManager* scn_man) : Scene(input, scn_man),
perlin_tex(nullptr),
perlin_mat(nullptr),
perlin_model(nullptr),
debug_cam(nullptr),
perlin_shader(nullptr)
{
	//debug_cam = new DebugCamera(60.0f, 1280, 720, 0.1f, 500.0f);
	//debug_cam->set_input(input);
	//debug_cam->set_fly_speed(3.0f);
	//debug_cam->set_mouse_sensitivity(1.5f);
}

PerlinScene::~PerlinScene()
{
	if(debug_cam) delete debug_cam;
	if(perlin_mat) delete perlin_mat;
	if(perlin_model) delete perlin_model;
	if(perlin_tex) delete perlin_tex;
}

int perlin_val(float x, float y)
{
	float scale = 0.04f;

	float perlin_val = fperlin_fractal(x*scale, y*scale, scale*56, 3, 0.55f);

	perlin_val = std::min(1.0f, std::max(-1.0f, perlin_val));
	perlin_val = (perlin_val + 1.0f) / 2.0f;
	int tex_val = (unsigned char)floor(perlin_val * 255.0f);

	return tex_val;
}

void PerlinScene::resized_window(int width, int height)
{
	vec3 lastpos = vec3(0,0,0);
	vec3 lastrot = vec3(0,0,0);
	if(debug_cam)
	{
		lastpos = debug_cam->get_position();
		lastrot = debug_cam->get_euler();
		delete debug_cam;
	}
	debug_cam = new DebugCamera(60.0f, width, height, 0.1f, 500.0f);
	debug_cam->set_input(input);
	debug_cam->set_fly_speed(3.0f);
	debug_cam->set_mouse_sensitivity(1.5f);
	debug_cam->set_position(lastpos);
	debug_cam->set_rotation(lastrot);
}

void PerlinScene::load(ShaderManager& sha_man, TextureManager& tex_man)
{
	perlin_shader = sha_man.get_shader("data/shaders/Voxel-vert.glsl", "data/shaders/Voxel-frag.glsl");


	perlin_mat = new Material();
	perlin_model = Primitives::create_cube();
	perlin_model->set_material(perlin_mat);


	// Generate texture
	const int width = 512;
	const int height = 512;
	unsigned char* data = new unsigned char[width * height * 3];// 3 bytes per pixel, RGB

	for(int i=0; i < width; i++)
	{
		for(int j=0; j < height; j++)
		{
			int index = i * 3 + j * height * 3;

			int tex_val = perlin_val(i,j);

			data[index] = tex_val;
			data[index+1] = tex_val;
			data[index+2] = tex_val;

		}
	}

	perlin_tex = new GLTexture(ColorFormat::RGB, data, width, height);

	// Apply texture
	perlin_mat->texture = perlin_tex;

}

void PerlinScene::init()
{
	input->set_lock_mouse(false);
	input->show_cursor(true);
}

void PerlinScene::unload()
{}

void PerlinScene::update(const float delta)
{
	debug_cam->update(delta);
}

void PerlinScene::render(RenderPass& pass)
{
	if(perlin_mat->texture)
	{
		perlin_mat->texture->dispose();
		delete perlin_mat->texture;
	}

	perlin_mat->texture = nullptr;

	// Generate texture
	const int width = 256;
	const int height = 256;
	unsigned char* data = new unsigned char[width * height * 3];// 3 bytes per pixel, RGB

	for(int i=0; i < width; i++)
	{
		for(int j=0; j < height; j++)
		{
			int index = i * 3 + j * height * 3;

			float x = i + (Time::time_since_startup_sec()* 100.0f);
			float y = j;

			int tex_val = perlin_val(x,y);

			data[index] = tex_val;
			data[index+1] = tex_val;
			data[index+2] = tex_val;

		}
	}

	perlin_tex = new GLTexture(ColorFormat::RGB, data, width, height);

	delete data;

	// Apply texture
	perlin_mat->texture = perlin_tex;

	assert(perlin_tex && perlin_mat && perlin_model && debug_cam);

	pass.draw_model(perlin_model, perlin_shader, debug_cam);
}

void PerlinScene::dispose()
{}