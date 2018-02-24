#include "game/MainScene.hpp"
#include "graphics/Material.hpp"
#include "core/Common.hpp"
#include "core/Input.hpp"
#include "graphics/RenderPass.hpp"
#include "graphics/ShaderManager.hpp"
#include "graphics/TextureManager.hpp"

#include "graphics/OrthographicCamera.hpp"
#include "graphics/ShaderProgram.hpp"
#include "game/DebugCamera.hpp"
#include "graphics/Primitives.hpp"

#include <SDL2/SDL.h>
#include <imgui/imgui.h>
#include <stb_image.h>
#include <algorithm>

#include "core/PerlinNoise.hpp"
#include "voxel/VoxelData.hpp"
#include "voxel/MarchingCubes.hpp"
#include "graphics/VertexArray.hpp"
#include "graphics/GLBuffer.hpp"
#include "core/Time.hpp"


MainScene::MainScene(Input* input, SceneManager* scene_manager) : Scene(input, scene_manager),
cam(nullptr),
shader(nullptr),
mat(nullptr),
test_loaded_model(nullptr)
{
}

MainScene::~MainScene()
{}

void MainScene::load(ShaderManager* sha_man, TextureManager* tex_man)
{
	// Voxel shader
	voxel_shader = sha_man->get_shader("data/shaders/Voxel-vert.glsl", "data/shaders/Voxel-frag.glsl");

	// Basic shader
	shader = sha_man->get_shader("data/shaders/Basic-vert.glsl", "data/shaders/Basic-frag.glsl");

	mat = new Material();
	mat->texture = tex_man->get_texture("data/textures/grass.jpg", ColorFormat::RGB);


	skybox_shader = sha_man->get_shader("data/shaders/Skybox-vert.glsl","data/shaders/Skybox-frag.glsl");
	skybox = load_obj_from_file("data/models/cubemapped_cube.obj");
	skybox->get_material()->texture = tex_man->get_texture("data/textures/Skybox.png", ColorFormat::RGB);
	skybox->transform.scale(100.0f,100.0f,100.0f);

	test_loaded_model = load_obj_from_file("data/models/Seeker_3.obj");
	delete test_loaded_model->get_material();
	test_loaded_model->set_material(mat);
	assert(test_loaded_model);
}

void MainScene::init()
{
	cam = new DebugCamera(60.0f, 1280, 720, 0.1f, 500.0f);
	cam->set_input(input);
	cam->set_fly_speed(3.0f);
	cam->set_mouse_sensitivity(1.5f);

	
	
	tmp = Primitives::create_cube();

	tmp->set_material(mat);

	myvoxels = new VoxelData(32,32);
	
	
	for(int i=0; i < myvoxels->get_width(); i++)
	{
		for(int j=0; j < myvoxels->get_height(); j++)
		{
			for(int k=0; k < myvoxels->get_width(); k++)
			{
				//glm::vec3 diff = glm::vec3((float)i,(float)j,(float)k) - glm::vec3((myvoxels->get_width()-1)*0.5f, (myvoxels->get_height()-1)*0.5f, (myvoxels->get_width()-1)*0.5f);
				
				
				//float len = diff.x*diff.x + diff.y*diff.y + diff.z*diff.z;
				//len = sqrt(len);
				//float tmp = len / ((myvoxels->get_width()-1));
				float tmp = fperlin_fractal(i * 0.13f,j * 0.13f,k * 0.13f, 2, 0.5f);
				tmp = std::min(1.0f,std::max(-1.0f,tmp));
				tmp = (tmp + 1.0f) / 2.0f;
				myvoxels->set_value_at_index(i,j,k, (unsigned char)(tmp*255));
				
				if( i == 0 || i == myvoxels->get_width()-1 ||
					j == 0 || j == myvoxels->get_height()-1 ||
					k == 0 || k == myvoxels->get_width()-1)
					myvoxels->set_value_at_index(i,j,k, 0);
			}
		}
	}//*/


	MarchingCubes* myCubes = new MarchingCubes();
	MCMesh* mesh = myCubes->Evaluate(myvoxels, 127);

	printf("Evaluated Marching cubes and got %i triangles\n",mesh->tri_count);

	voxel_model = new Model();
	voxel_model->set_vertices(mesh->vertices);
	voxel_model->set_texcoords(mesh->texcoords);
	voxel_model->set_normals(mesh->normals);	
	voxel_model->set_material(mat);
	voxel_model->transform.translate(-5,0,0);

	
	delete mesh; // You need to clean up mesh after it's created
	delete myCubes;
}

void MainScene::unload()
{}

void MainScene::update(const float delta)
{
	if(input->is_mouse_locked() != cam->is_mouse_input_active())
		cam->set_mouse_input_active(input->is_mouse_locked());

	cam->update(delta);

	ImGui::Begin("MainScene info");
	ImGui::Text("Deltatime %.4f", delta);
	ImGui::Text("FPS: %.0f",(1.0f / delta));
	ImGui::End();
}

void MainScene::render(RenderPass* pass)
{
	voxel_model->dispose();
	delete voxel_model;
	MarchingCubes* myCubes = new MarchingCubes();
	MCMesh* mesh = myCubes->Evaluate(myvoxels, 127);

	for(int i=0; i < myvoxels->get_width(); i++)
	{
		for(int j=0; j < myvoxels->get_height(); j++)
		{
			for(int k=0; k < myvoxels->get_width(); k++)
			{
				//glm::vec3 diff = glm::vec3((float)i,(float)j,(float)k) - glm::vec3((myvoxels->get_width()-1)*0.5f, (myvoxels->get_height()-1)*0.5f, (myvoxels->get_width()-1)*0.5f);
				float time = ((float)Time::time_since_startup_sec() * 10.0f);
				//int time = 0;
				float x = i + time;
				//float len = diff.x*diff.x + diff.y*diff.y + diff.z*diff.z;
				//len = sqrt(len);
				//float tmp = len / ((myvoxels->get_width()-1));
				float tmp = fperlin_fractal(x * 0.13f,j * 0.13f,k * 0.13f, 2, 0.5f);
				tmp = std::min(1.0f,std::max(-1.0f,tmp));
				tmp = (tmp + 1.0f) / 2.0f;
				myvoxels->set_value_at_index(i,j,k, (unsigned char)(tmp*255));
				
				if( i == 0 || i == myvoxels->get_width()-1 ||
					j == 0 || j == myvoxels->get_height()-1 ||
					k == 0 || k == myvoxels->get_width()-1)
					myvoxels->set_value_at_index(i,j,k, 0);
			}
		}
	}//*/
	
	voxel_model = new Model();
	voxel_model->set_vertices(mesh->vertices);
	voxel_model->set_texcoords(mesh->texcoords);
	voxel_model->set_normals(mesh->normals);	
	voxel_model->set_material(mat);
	voxel_model->transform.translate(-5,0,0);

	delete mesh;
	delete myCubes;


	assert(mat->texture);
	static bool wire = false;
	
	ImGui::Begin("MainScene info");
	if(ImGui::Checkbox("Wireframe", &wire)) 
	{
		if(wire)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	ImGui::End();

	pass->draw_model(skybox, skybox_shader, cam);

	pass->draw_model(tmp, shader, cam);

	pass->draw_model(voxel_model, voxel_shader, cam);

	if(test_loaded_model)
		pass->draw_model(test_loaded_model, shader, cam);

	//bool val = true;
	//ImGui::ShowDemoWindow(&val);

}

void MainScene::dispose()
{
	Scene::dispose();
	tmp->dispose();
	delete tmp;
	delete mat;
	delete cam;
}



