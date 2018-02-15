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

#include "voxel/VoxelData.hpp"
#include "voxel/MarchingCubes.hpp"
#include "graphics/VertexArray.hpp"



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

	// Voxel shader
	vert = read_file("data/shaders/Voxel-vert.glsl");
	frag = read_file("data/shaders/Voxel-frag.glsl");
	voxel_shader = new ShaderProgram(vert.c_str(), frag.c_str(), header.c_str());

	tmp = Primitives::create_cube();

	mat = new Material();
	mat->texture = load_image("data/textures/grass.jpg");
	
	tmp->set_material(mat);

	myvoxels = new VoxelData(16,16);
	
	
	for(int i=0; i < myvoxels->get_width(); i++)
	{
		for(int j=0; j < myvoxels->get_height(); j++)
		{
			for(int k=0; k < myvoxels->get_width(); k++)
			{
				glm::vec3 diff = glm::vec3(i,j,k) - glm::vec3(myvoxels->get_width(), myvoxels->get_height(), myvoxels->get_width())*0.5f;
				
				//if(diff.length() < 4.5f)// || j % 2 == 0 || i % 2 == 0)
				//	myvoxels->set_value_at_index(k,j,i, 250);
				//else
				float len = diff.x*diff.x + diff.y*diff.y + diff.z*diff.z;
				len = sqrt(len);
				float tmp = len / myvoxels->get_width();
				myvoxels->set_value_at_index(i,j,k, (unsigned char)(tmp*225));
				
			}
		}
	}//*/

	//myvoxels->set_value_at_index(2,2,2, 250);
	//myvoxels->set_value_at_index(3,3,3, 250);

	std::vector<glm::vec3> vertices;
	MarchingCubes* myCubes = new MarchingCubes();
	int tris = myCubes->Evaluate(myvoxels, 128, &vertices);
	cout("Evaluated Marching cubes and got ");
	cout(tris);
	coutln(" triangles in resulting mesh");

	//for(int i=0; i < vertices.size(); i++)
	//	printf("V:(%.1f,%.1f,%.1f)\n",vertices[i].x,vertices[i].y,vertices[i].z);

	voxel_model = new Model();
	voxel_model->set_vertices(vertices);
	voxel_model->set_material(mat);
	voxel_model->transform.translate(-5,0,0);

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
	mat->dispose();
	delete tmp;
	delete mat;
	delete shader;
	delete cam;
}



