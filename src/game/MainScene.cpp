#include "game/MainScene.hpp"
#include "graphics/Primitives.hpp"
#include "graphics/RenderPass.hpp"
#include "graphics/ShaderProgram.hpp"
#include "game/DebugCamera.hpp"
#include "core/Files.hpp"
#include "graphics/Material.hpp"
#include "core/Macros.hpp"
#include "core/Input.hpp"


#include "graphics/OrthographicCamera.hpp"

#include <SDL2/SDL.h>
#include <imgui/imgui.h>
#include <stb_image.h>
#include <algorithm>

#include "graphics/ShaderManager.hpp"
#include "voxel/VoxelData.hpp"
#include "voxel/MarchingCubes.hpp"
#include "graphics/VertexArray.hpp"
#include "graphics/GLBuffer.hpp"



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
	cam->set_mouse_sensitivity(1.5f);

	// Basic shader
	std::string vert = read_file("data/shaders/Basic-vert.glsl");
	std::string frag = read_file("data/shaders/Basic-frag.glsl");
	std::string header = read_file("data/shaders/Shader_Header.glsl");
	shader = new ShaderProgram(vert.c_str(), frag.c_str(), header.c_str());
	//shader = ShaderManager::get_shader("data/shaders/Basic-vert.glsl", "data/shaders/Basic-frag.glsl");

	// Voxel shader
	vert = read_file("data/shaders/Voxel-vert.glsl");
	frag = read_file("data/shaders/Voxel-frag.glsl");
	//header = read_file("data/shaders/Shader_Header.glsl");
	voxel_shader = new ShaderProgram(vert.c_str(), frag.c_str(), header.c_str());
	//voxel_shader = ShaderManager::get_shader("data/shaders/Voxel-vert.glsl", "data/shaders/Voxel-frag.glsl");

	tmp = Primitives::create_cube();

	mat = new Material();
	mat->texture = load_image("data/textures/grass.jpg");
	
	tmp->set_material(mat);

	myvoxels = new VoxelData(32,32);
	
	
	for(int i=0; i < myvoxels->get_width(); i++)
	{
		for(int j=0; j < myvoxels->get_height(); j++)
		{
			for(int k=0; k < myvoxels->get_width(); k++)
			{
				glm::vec3 diff = glm::vec3((float)i,(float)j,(float)k) - glm::vec3((myvoxels->get_width()-1)*0.5f, (myvoxels->get_height()-1)*0.5f, (myvoxels->get_width()-1)*0.5f);
				
				
				float len = diff.x*diff.x + diff.y*diff.y + diff.z*diff.z;
				len = sqrt(len);
				float tmp = len / ((myvoxels->get_width()-1));
				tmp = std::min(1.0f,std::max(0.0f,tmp));
				tmp = 1.0f - tmp;
				myvoxels->set_value_at_index(i,j,k, (unsigned char)(tmp*255));
				
				if( i % 4 == 0 || k % 4 == 0 ) myvoxels->set_value_at_index(i,j,k, 0);
			}
		}
	}//*/


	MarchingCubes* myCubes = new MarchingCubes();
	MCMesh* mesh = myCubes->Evaluate(myvoxels, 127);

	cout("Evaluated Marching cubes and got ");
	cout(mesh->tri_count);
	coutln(" triangles in resulting mesh");

	voxel_model = new Model();
	voxel_model->set_vertices(mesh->vertices);
	voxel_model->set_texcoords(mesh->texcoords);
	voxel_model->set_normals(mesh->normals);	
	voxel_model->set_material(mat);
	voxel_model->transform.translate(-5,0,0);

	delete mesh; // You need to clean up mesh after it's created
	delete myCubes;
}

void MainScene::load()
{}

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


	pass->draw_model(tmp, shader, cam);

	pass->draw_model(voxel_model, voxel_shader, cam);


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



